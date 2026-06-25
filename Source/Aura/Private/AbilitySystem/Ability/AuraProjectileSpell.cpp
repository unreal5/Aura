// 版权没有，拷贝自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/AuraGlobalTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	check(ProjectileClass);

	auto ActivationInfo = GetCurrentActivationInfo();
	if (!HasAuthority(&ActivationInfo)) return;

	const FVector WeaponSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo());

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(WeaponSocketLocation);
	auto Rotation = (TargetLocation - WeaponSocketLocation).ToOrientationRotator();
	Rotation.Pitch = 0.f;
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AActor* Owner = GetOwningActorFromActorInfo();
	APawn* InstigatorPawn = Cast<APawn>(Owner);
	ESpawnActorCollisionHandlingMethod SpawnMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (auto ProjectileInst = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, SpawnTransform, Owner, InstigatorPawn, SpawnMethod))
	{
		// 创建DamageEffectSpecHandle
		if (DamageEffectClass)
		{
			auto SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
			// 计算伤害值并赋值给SpecHandle
			const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Incoming::Damage, ScaledDamage);
			ProjectileInst->DamageEffectSpecHandle = SpecHandle;
		}
		ProjectileInst->FinishSpawning(SpawnTransform);
	}
}