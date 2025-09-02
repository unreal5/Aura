// 版权没有，拷贝自由。


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect>& GameplayEffectClass)
{
	check(GameplayEffectClass);

	const auto Asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!Asc) return;

	FGameplayEffectContextHandle ContextHandle = Asc->MakeEffectContext();
	//ContextHandle.AddSourceObject(this);
	ContextHandle.AddInstigator(this, this);
	const FGameplayEffectSpecHandle SpecHandle = Asc->MakeOutgoingSpec(GameplayEffectClass, 1, ContextHandle);
	if (SpecHandle.IsValid())
	{
		Asc->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetAcotr)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetAcotr, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetAcotr, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetAcotr, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// Remove infinite effects
	
}
