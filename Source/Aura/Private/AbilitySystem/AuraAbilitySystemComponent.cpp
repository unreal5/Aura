// 版本没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Ability/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (auto AbilityClass : StartupAbilities)
	{
		if (!IsValid(AbilityClass)) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		if (auto AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability.Get()))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto&& AbilitySpec : GetActivatableAbilities())
	{
		// 与输入Tag无关
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		// 告诉技能与其相关的输入正在被按下，这个函数会对输入保持跟踪
		// 对已经激活的技能，会最终调用UGameplayAbility::InputPressed虚函数。
		AbilitySpecInputPressed(AbilitySpec);
		
		// 不激活已经激活的技能
		if (AbilitySpec.IsActive()) continue;
		
		// 激活技能
		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto&& AbilitySpec : GetActivatableAbilities())
	{
		// 与输入Tag无关
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		// 给技能一个机会处理输入释放
		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* SourceASC,
                                                  const FGameplayEffectSpec& EffectSpec,
                                                  FActiveGameplayEffectHandle ActiveHandle) const
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTagsDelegate.Broadcast(TagContainer);
}
