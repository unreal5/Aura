// 版权没有，拷贝自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "InterAction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	// Only the server should spawn the projectile
	const auto AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor) return;
	if (!AvatarActor->Implements<UCombatInterface>()) return;
	if (!AvatarActor->HasAuthority()) return;

	FTransform ProjectileTransform = FTransform::Identity;
	FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(AvatarActor);
	FRotator ProjectileRotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	ProjectileTransform.SetLocation(SocketLocation);
	ProjectileTransform.SetRotation(ProjectileRotation.Quaternion());

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, ProjectileTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Projectile && IsValid(DamageEffectClass))
	{
		auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		auto EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
		int32 Level = GetAbilityLevel();
		auto SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, Level, EffectContextHandle);
		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(ProjectileTransform);
	}
}
