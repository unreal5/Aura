// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsDelegate, const FGameplayTagContainer& /*AssetTags*/);

UCLASS(ClassGroup=("UserComponent"), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAuraAbilitySystemComponent();
	void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	// 在用户调用InitAbilityActorInfo后调用，子类可以重写这个函数来绑定GE委托
	virtual void PostUserInitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);
	FEffectAssetTagsDelegate OnEffectAssetTagsAppliedDelegate;

	void AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& AbilitiesToAdd);

	// 对于激活技能，只关注两种按键：hold和release.因为有些技能需要持续按住才能保持激活状态，而有些技能则在按键释放时触发效果。
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
	                   FActiveGameplayEffectHandle ActiveEffectHandle);
	void EffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect);
};