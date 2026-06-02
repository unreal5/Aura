// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "Engine/World.h"


// Sets default values for this component's properties
UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// 只有在服务器上才绑定GE委托
	if (GetOwnerActor() && GetOwnerActor()->HasAuthority())
	{
		OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
		OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UAuraAbilitySystemComponent::EffectRemoved);
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle ActiveEffectHandle)
{
	UE_LOG(LogTemp, Warning, TEXT("Effect applied: %s"), *EffectSpec.Def->GetName());
}

void UAuraAbilitySystemComponent::EffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect)
{
	const FGameplayEffectSpec& Spec = ActiveGameplayEffect.Spec;
	const UGameplayEffect* EffectDef = Spec.Def;
	const FString EffectName = GetNameSafe(EffectDef);
	const int32 StackCount = GetCurrentStackCount(ActiveGameplayEffect.Handle);
	const float Duration = Spec.GetDuration();
	const float Period = Spec.GetPeriod();

	UE_LOG(LogTemp, Warning, TEXT("移除Active Effect: %s, StackCount: %d, Duration: %.2f,  Period: %.2f"),
	       *EffectName, StackCount, Duration, Period);
}
