// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"

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
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);

		// 对于定制的AuraGameplayAbility才有对应的StartupInputTag。
		if (const auto* AuraAbility = AbilityClass->GetDefaultObject<UAuraGameplayAbility>())
		{
			// Ability spec有GameplayTagContainer，可以用来绑定输入。
			// AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);

			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 检查所有可激活技能，如果已经激活，则忽略（不重复激活），否则激活。
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive()) continue;
			
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 检查所有可激活技能，如果已经激活，则忽略（不重复激活），否则激活。
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}
