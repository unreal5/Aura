// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "Character/AuraCharacterBase.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAuraAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	// ASC的InitializeComponent也会调用InitAbilityActorInfo，所以要防止在InitializeComponent中调用InitAbilityActorInfo时重复绑定GE委托

	if (InAvatarActor && InAvatarActor->IsA(AAuraCharacterBase::StaticClass()))
	{
		PostUserInitAbilityActorInfo(InOwnerActor, InAvatarActor);
	}
}

void UAuraAbilitySystemComponent::PostUserInitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	// 只有在服务器上才绑定GE委托

	if (GetOwnerActor() && GetOwnerActor()->HasAuthority())
	{
		/*
		 * RemoveAll:只删你的 UAuraAbilitySystemComponent 自己加进去的监听其他对象绑定的监听不会受影响
		 * 但如果别的系统也订阅了这个委托，例如 UI、调试工具、别的组件，它们不会被删掉。
		 * 
		 * Clear()含义是：清空整个委托列表中的所有绑定。也就是不管是谁绑的，统统删掉。
		 */
		OnGameplayEffectAppliedDelegateToSelf.RemoveAll(this);
		OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
		OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UAuraAbilitySystemComponent::EffectRemoved);
	}
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);
	OnEffectAssetTagsAppliedDelegate.Broadcast(AssetTags);
	

}

void UAuraAbilitySystemComponent::EffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect)
{
	// const FGameplayEffectSpec& Spec = ActiveGameplayEffect.Spec;
	// const UGameplayEffect* EffectDef = Spec.Def;
	// const FString EffectName = GetNameSafe(EffectDef);
	// const int32 StackCount = GetCurrentStackCount(ActiveGameplayEffect.Handle);
	// const float Duration = Spec.GetDuration();
	// const float Period = Spec.GetPeriod();
}
