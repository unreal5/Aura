// 版权没有，拷贝自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(ProjectileClass);

	if (!HasAuthority(&ActivationInfo)) return;

	const FVector WeaponSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo());
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(WeaponSocketLocation);

	AActor* Owner = GetOwningActorFromActorInfo();
	APawn* InstigatorPawn = Cast<APawn>(Owner);
	ESpawnActorCollisionHandlingMethod SpawnMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (auto ProjectileInst = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, SpawnTransform, Owner, InstigatorPawn, SpawnMethod))
	{
		ProjectileInst->FinishSpawning(SpawnTransform);
	}
	
	K2_EndAbility();
}