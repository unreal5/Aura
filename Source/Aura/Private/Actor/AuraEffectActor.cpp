// 版权没有，拷贝自由。


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Elements/Common/TypedElementQueryDescription.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("场景组件")));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC) return;

	FGameplayEffectContextHandle GameplayEffectContext = TargetASC->MakeEffectContext();
	GameplayEffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetASC->MakeOutgoingSpec(
		InGameplayEffectClass, ActorLevel, GameplayEffectContext);
	if (GameplayEffectSpecHandle.IsValid())
	{
		// TargetASC 应用效果到自身，因此如果堆叠，则AggregateBySource和ByTarget一样，源和目标都是按ASC来区分的。
		auto ActivateHanlde = TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data);

		//UE_LOG(LogTemp, Display, TEXT("Duration =%f"), GameplayEffectSpecHandle.Data->GetDuration());

		// if (ActivateHanlde.WasSuccessfullyApplied())
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("应用效果时间:%f"), GetWorld()->GetTimeSeconds());
		// }
		//{
		//	UE_LOG(LogTemp, Display, TEXT("应用效果成功"));
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Display, TEXT("应用效果失败"));
		//}

		/* 研究以下代码*/
		/**
		FGameplayEffectQuery Query;
		Query.CustomMatchDelegate.BindLambda([&](const FActiveGameplayEffect& CurEffect)
		{
			bool bMatches = false;

			// First check at matching: backing GE class must be the exact same
			if (CurEffect.Spec.Def && InGameplayEffectClass == CurEffect.Spec.Def->GetClass())
			{
				// If an instigator is specified, matching is dependent upon it
				if (TargetASC)
				{
					bMatches = (TargetASC == CurEffect.Spec.GetEffectContext().GetInstigatorAbilitySystemComponent());
				}
				else
				{
					bMatches = true;
				}
			}

			return bMatches;
		});
		float EndTime;
		float Duration;
		TargetASC->GetActiveEffectsEndTimeAndDuration(Query, EndTime, Duration);
		UE_LOG(LogTemp, Display, TEXT("EndTime =%f, Duration = %f"), EndTime, Duration);
		*/
	}
}

void AAuraEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	// ApplyEffectToTarget已经做了有效性检查
	if (InstantApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}


	if (InfiniteApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// 检测是否结束重叠时应用效果
	if (InstantApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}


	if (InfiniteApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// 对于永久效果，做额外检查
	if (InfiniteRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		if (auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			TargetASC->RemoveActiveGameplayEffectBySourceEffect(InfiniteGameplayEffectClass, TargetASC, 1);
		}
	}
}
