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

	const auto TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetAsc) return;

	FGameplayEffectContextHandle ContextHandle = TargetAsc->MakeEffectContext();
	//ContextHandle.AddSourceObject(this);
	ContextHandle.AddInstigator(this, this);
	const FGameplayEffectSpecHandle SpecHandle = TargetAsc->MakeOutgoingSpec(GameplayEffectClass, 1, ContextHandle);
	if (SpecHandle.IsValid())
	{
		auto ActiveEffectHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		const bool bIsInfinite = SpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveEffectHandles.Add(ActiveEffectHandle, TargetAsc);
		}
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
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		auto TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		if (!IsValid(TargetAsc)) return;
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		// Find all active effect handles that belong to the TargetAsc
		// and remove them
		// We cannot remove them directly in the loop as we are iterating over the map
		// So we store them in a temporary array and remove them after the loop
		// This is to avoid modifying the map while iterating over it
		// which can lead to undefined behavior
		for (const auto[ActiveEffectHandle, Asc] : ActiveEffectHandles)
		{
			if (TargetAsc == Asc)
			{
				TargetAsc->RemoveActiveGameplayEffect(ActiveEffectHandle, 1);
				HandlesToRemove.Add(ActiveEffectHandle);
			}
		}
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
