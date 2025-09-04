// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemComponent.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* Asc, const FGameplayEffectSpec& SpecApplied,
                                                FActiveGameplayEffectHandle ActiveHandle)
{
	UE_LOG(LogTemp, Warning, TEXT("Effect Applied: %s"), *SpecApplied.Def.GetName());
}
