// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Ability/AuraGameplayAbility.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	/** Called on server whenever a GE is applied to someone else. This includes instant and duration based GEs. */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InStartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& StartupAbility : InStartupAbilities)
	{
		if (!IsValid(StartupAbility)) continue;

		auto AbilitySpec = FGameplayAbilitySpec(StartupAbility, 1);
		if (const auto AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 只对没有激活的技能才尝试激活
	for (auto& Spec : GetActivatableAbilities())
	{
		// 告诉所有的可激活技能，有一个按键被按下了（设置对应的按键按下标志，如果已经激活还会调用InputPressed()函数）。
		// 参考技能的连招逻辑的实现。
		AbilitySpecInputPressed(Spec);
		// 已经激活的技能不需要再尝试激活
		if (Spec.IsActive()) continue;

		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			TryActivateAbility(Spec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 只对没有激活的技能才尝试激活
	for (auto& Spec : GetActivatableAbilities())
	{
		AbilitySpecInputReleased(Spec);
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* Asc, const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle ActiveHandle) const
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
