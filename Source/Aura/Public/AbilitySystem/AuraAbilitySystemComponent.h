// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

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
protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	void EffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect);
};
