// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer&);

using FEffectAssetTagsDelegate = TMulticastDelegate<void(const FGameplayTagContainer&)>;

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	/** Called on server whenever a GE is applied to self. This includes instant and duration based GEs. */
	FEffectAssetTagsDelegate OnEffectAssetTagsDelegate;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* SourceASC, const FGameplayEffectSpec& EffectSpec,
	                     FActiveGameplayEffectHandle ActiveHandle) const;
};
