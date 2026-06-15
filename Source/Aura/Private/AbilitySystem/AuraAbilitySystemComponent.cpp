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
		// GameplayAbility 要满足运行时改变StartupInputTag的条件，这样可以动态的关联按键与Ability。
		// 1. 不能在运行时修改默认对象的属性，因为它会影响所有使用这个类的实例。
		// 2. 只能在运行时修改AbilitySpec的属性，因为它是每个实例独有的。
		FGameplayTag InputTag = Ability.GetDefaultObject()->StartupInputTag;
		if (InputTag.IsValid())
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(InputTag);
		}
		GiveAbility(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 对于未激活的技能要激活，如果已经激活了就继续保持激活状态。
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTag(InputTag)) continue;

		// 不要丢失原来的行为，当前技能激活与否，都不能改变又有同样的InputTag的技能的输入状态。
		AbilitySpecInputPressed(AbilitySpec);

		if (!AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;


	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTag(InputTag)) continue;

		AbilitySpecInputReleased(AbilitySpec);
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