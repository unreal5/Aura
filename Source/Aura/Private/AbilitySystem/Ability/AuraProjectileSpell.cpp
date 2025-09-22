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

	// Only the server should spawn the projectile
	if (!HasAuthority(&ActivationInfo)) return;
	
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor->Implements<UCombatInterface>()) return;

	FTransform ProjectileTransform = FTransform::Identity;
	ProjectileTransform.SetLocation(ICombatInterface::Execute_GetCombatSocketLocation(AvatarActor));
	//TODO: set rotation based on aiming direction
	
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, ProjectileTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(ActorInfo->AvatarActor.Get()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Projectile)
	{
		//TODO: set projectile's GameplayEffect for causing damage
		Projectile->FinishSpawning(ProjectileTransform);
	}
}
