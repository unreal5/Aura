// 版权没有，拷贝自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "InterAction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation) {
	// Only the server should spawn the projectile
	const auto AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor) return;
	if (!AvatarActor->Implements<UCombatInterface>()) return;
	if (!AvatarActor->HasAuthority()) return;

	FTransform ProjectileTransform = FTransform::Identity;
	ProjectileTransform.SetLocation(ICombatInterface::Execute_GetCombatSocketLocation(AvatarActor));
	//TODO: set rotation based on aiming direction
	
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, ProjectileTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Projectile)
	{
		//TODO: set projectile's GameplayEffect for causing damage
		Projectile->FinishSpawning(ProjectileTransform);
	}
}
