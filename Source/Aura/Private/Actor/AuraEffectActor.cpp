// 版权没有，拷贝自由。


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "Engine/World.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


bool AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC || !InGameplayEffectClass) return false;

	auto EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	auto SpecHandle = TargetASC->MakeOutgoingSpec(InGameplayEffectClass, ActorLevel, EffectContextHandle);

	if (SpecHandle.IsValid())
	{
		auto ActivateHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		return ActivateHandle.WasSuccessfullyApplied();
	}
	return false;
}

void AAuraEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	if (IsValid(InstantGameplayEffectClass) &&
		InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);

		if (bDestroyOnEffectRemoval) 
			Destroy();
	}

	if (IsValid(DurationGameplayEffectClass) &&
		DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
		
		if (bDestroyOnEffectRemoval) 
			Destroy();
	}

	if (IsValid(InfiniteGameplayEffectClass) &&
		InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		
		if (bDestroyOnEffectRemoval) 
			Destroy();
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (IsValid(InstantGameplayEffectClass) && InstantEffectApplicationPolicy ==
		EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
		
		if (bDestroyOnEffectRemoval) 
			Destroy();
	}

	if (IsValid(DurationGameplayEffectClass) &&
		DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
		
		if (bDestroyOnEffectRemoval) 
			Destroy();
	}

	if (IsValid(InfiniteGameplayEffectClass) &&
		InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		
		if (bDestroyOnEffectRemoval) 
			Destroy();
	}
	// 结束重叠时，按要求移除无限效果
	if (IsValid(InfiniteGameplayEffectClass) &&
		InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemovalOnEndOverlap)
	{
		auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (TargetASC)
		{
			TargetASC->RemoveActiveGameplayEffectBySourceEffect(InfiniteGameplayEffectClass, TargetASC, 1);
		}
	}
}
