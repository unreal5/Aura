// 版本没有，拷贝自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	checkf(ProjectileClass, TEXT("ProjectileClass is nullptr."));
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTage)
{
	auto Avatar = GetAvatarActorFromActorInfo();
	check(Avatar);

	if (!Avatar->HasAuthority()) return;
	if (!Avatar->Implements<UCombatInterface>()) return;
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		Avatar, SocketTage);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform.SetRotation(Rotation.Quaternion());

	auto AuraProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (AuraProjectile)
	{
		SpawnTransform.SetLocation(SocketLocation);
		auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Avatar);
		auto SourceContextHandle = SourceASC->MakeEffectContext();
		SourceContextHandle.AddSourceObject(AuraProjectile);
		SourceContextHandle.SetAbility(this);

		auto SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceContextHandle);

		for (const auto& [Key, Value] : DamageTypes)
		{
			const float ScaledDamage = Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Key,
			                                                              ScaledDamage);
		}

		AuraProjectile->DamageEffectSpecHandle = SpecHandle;

		AuraProjectile->FinishSpawning(SpawnTransform);
	}
}