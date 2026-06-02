// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAuraAbilitySystemComponent();
	void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	// 在用户调用InitAbilityActorInfo后调用，子类可以重写这个函数来绑定GE委托
	// 注意 InitAbilityActorInfo可能会被多次调用，所以要确保绑定GE委托的代码只会执行一次
	virtual void PostUserInitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);
protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	void EffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect);
};
