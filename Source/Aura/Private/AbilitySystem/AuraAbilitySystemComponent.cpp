// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Ability/AuraGameplayAbility.h"
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
		 * RemoveAll:只删自己加进去的监听其他对象绑定的监听不会受影响
		 * Clear()：清空整个委托列表中的所有绑定。也就是不管是谁绑的，统统删掉。
		 */
		OnGameplayEffectAppliedDelegateToSelf.RemoveAll(this);
		OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);

		OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
		OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UAuraAbilitySystemComponent::EffectRemoved);
	}
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(
	const TArray<TSubclassOf<UAuraGameplayAbility>>& AbilitiesToAdd)
{
	if (!GetOwnerActor() || !GetOwnerActor()->HasAuthority())
	{
		check(false && "Only the server can add abilities.");
		return;
	}
	int32 PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(GetOwnerActor());

	for (const TSubclassOf<UAuraGameplayAbility>& Ability : AbilitiesToAdd)
	{
		if (!IsValid(Ability)) continue;


		FGameplayAbilitySpec AbilitySpec(Ability, PlayerLevel, INDEX_NONE);
		//GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce(AbilitySpec);
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