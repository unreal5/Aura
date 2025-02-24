// 版权没有，拷贝自由。


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

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
void AAuraCharacterBase::ApplyGameplayEffectClassToSelf(const TSubclassOf<UGameplayEffect>& InAppliedGameplayEffect, float Level) const
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

void AAuraCharacterBase::InitializeDefaultAttributes()
{
	ApplyGameplayEffectClassToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyGameplayEffectClassToSelf(DefaultSecondaryAttributes, 1.f);
}
