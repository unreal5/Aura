// 版本没有，拷贝自由。


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent")));
	// 网络同步
	// 预初始化，要直接设置，不要调用函数
	// SetReplicates(true);
	bReplicates = true;
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	const bool bIsEnemy = TargetActor->ActorHasTag("Enemy");
	if (bIsEnemy && !bApplyEffectsToEnemies) return;

	auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;


	// 使用Context传递数据
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	// 这个Effect的来源
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, ActorLevel, EffectContext);

	check(EffectSpecHandle.IsValid());

	auto ActivatedEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

	/* @Arthu的方法，不需要保存Handle
	const bool bIsInfinite = EffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlay)
	{
		ActiveEffectHandles.Add(ActivatedEffectHandle, TargetASC);
	}
	*/

	// else
	// {
	// 	// 关于GE的最新说明：
	// 	// https://dev.epicgames.com/documentation/zh-cn/unreal-engine/gameplay-effects-for-the-gameplay-ability-system-in-unreal-engine?application_version=5.4
	// 	UE_LOG(LogTemp, Error, TEXT("EffectSpecHandle is not valid at %hs:%d"), __FUNCTION__, __LINE__);
	// }

	if (EffectSpecHandle.Data->Def->DurationPolicy != EGameplayEffectDurationType::Infinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnOverlay(AActor* TargetActor)
{
	const bool bIsEnemy = TargetActor->ActorHasTag("Enemy");
	if (bIsEnemy && !bApplyEffectsToEnemies) return;
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlay(AActor* TargetActor)
{
	const bool bIsEnemy = TargetActor->ActorHasTag("Enemy");
	if (bIsEnemy && !bApplyEffectsToEnemies) return;
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// 移除策略
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlay)
	{
		// 需要EffectHandle，才可以移除
		auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		// 以下方法来自：https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg/learn/lecture/39783288#questions/21466542
		// 暂时不用，因为要跟上教程
		/**/
		TargetASC->RemoveActiveGameplayEffectBySourceEffect(InfiniteGameplayEffectClass, TargetASC, 1);


		/*
		// TMap不可以直接移除，否则会导致迭代器失效
		TArray<FActiveGameplayEffectHandle> KeysToRemove;
		for (auto [Key, Value] : ActiveEffectHandles)
		{
			if (Value == TargetASC)
			{
				// 当前正在迭代，不可以直接移除，簿记一下
				TargetASC->RemoveActiveGameplayEffect(Key, 1);
				KeysToRemove.Add(Key);
			}
		}
		if (!KeysToRemove.IsEmpty())
		{
			for (auto& Key : KeysToRemove)
			{
				ActiveEffectHandles.FindAndRemoveChecked(Key);
			}
		}
		*/
	}
}