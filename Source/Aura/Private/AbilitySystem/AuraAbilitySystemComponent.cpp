// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	auto AppliedDelegate = [this](UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec,
	                              FActiveGameplayEffectHandle Handle)
	{
		FGameplayTagContainer AssetTagContainer;
		Spec.GetAllAssetTags(AssetTagContainer);
		if (AssetTagContainer.IsEmpty()) return;
		
		EffectAssetTags.Broadcast(AssetTagContainer);
	};

	// Called on server whenever a GE is applied to self. This includes instant and duration based GEs
	OnGameplayEffectAppliedDelegateToSelf.AddLambda(AppliedDelegate);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const auto& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec Spec(AbilityClass, 1);
		//GiveAbility(Spec);
		GiveAbilityAndActivateOnce(Spec);
	}
}
