// 版权没有，拷贝自由。




#include "AbilitySystem/Ability/AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	auto Avatar = GetAvatarActorFromActorInfo();

	if (!Avatar || !Avatar->HasAuthority()) return;


	if (auto World = GetWorld())
	{
		AActor* Owner = GetOwningActorFromActorInfo();
		APawn* Instigator = GetOwningActorFromActorInfo()->GetInstigator();

		ICombatInterface* CombatInterface = Cast<ICombatInterface>(Avatar);
		if (!CombatInterface)
		{
			return;
		}
		FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation, ProjectileTargetLocation);
		LookAtRotation.Pitch = 0;
		SpawnTransform.SetRotation(LookAtRotation.Quaternion());

		auto Projectile = World->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, Owner, Instigator,
		                                                             ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (Projectile)
		{
			// 设置伤害效果
			auto ASC = GetAbilitySystemComponentFromActorInfo();
			auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Avatar);
			if (SourceASC)
			{
				FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
				ContextHandle.AddSourceObject(this);
				FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);
				if (EffectSpecHandle.IsValid())
				{
					Projectile->DamageEffectSpecHandle = EffectSpecHandle;
				}
			}
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
