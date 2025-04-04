// 版权没有，拷贝自由。


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);
	// 对于网络专门服务器特别重要，如果不设置，专门服务器不刷新骨骼位置。而我们依赖Montage发送事件。
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("武器"));
	Weapon->SetupAttachment(GetMesh(), TEXT("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAuraCharacterBase::GetCombatSocketLocation() const
{
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}


void AAuraCharacterBase::InitAbilityActorInfo()
{
}

// void AAuraCharacterBase::InitializePrimaryAttributes() const
// {
// 	ApplyGameplayEffectClassToSelf(DefaultPrimaryAttributes, 1.f);
// }
//
// void AAuraCharacterBase::InitializeSecondaryAttributes() const
// {
// 	ApplyGameplayEffectClassToSelf(DefaultSecondaryAttributes, 1.f);
// }
void AAuraCharacterBase::ApplyGameplayEffectClassToSelf(const TSubclassOf<UGameplayEffect>& InAppliedGameplayEffect,
                                                        float Level) const
{
	check(AbilitySystemComponent);
	check(InAppliedGameplayEffect);


	auto Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->
		MakeOutgoingSpec(InAppliedGameplayEffect, Level, Context);
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	int32 PlayerLevel = GetPlayerLevel();
	ApplyGameplayEffectClassToSelf(DefaultPrimaryAttributes, PlayerLevel);
	ApplyGameplayEffectClassToSelf(DefaultSecondaryAttributes, PlayerLevel);
	ApplyGameplayEffectClassToSelf(DefaultVitalAttributes, PlayerLevel);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority()) return;

	// grant ability to self
	if (auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->AddCharacterAbilities(StartupAbilities);
	}
}
