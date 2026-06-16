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
	// ClientRPC函数，会在服务器上调用，在Owning 客户端上执行。Reliable表示这个RPC必须可靠地传输，不能丢失。
	// 使用Client的原因：OnGameplayEffectAppliedDelegateToSelf是服务器上触发的，但我们需要在客户端上也处理这个事件，
	// 比如显示一些特效或者UI反馈，所以需要通过ClientRPC将这个事件传递到客户端。
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
	                   FActiveGameplayEffectHandle ActiveEffectHandle);
	UFUNCTION(Client, Reliable)
	void ClientEffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect);
};