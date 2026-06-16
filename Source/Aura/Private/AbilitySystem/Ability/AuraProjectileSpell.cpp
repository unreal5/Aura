// 版权没有，拷贝自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FVector AxisLoc = ActorInfo->AvatarActor.Get()->GetActorLocation();
	FRotator AxisRot = ActorInfo->AvatarActor.Get()->GetActorRotation();
	FVector AxisX = AxisRot.Vector();
	FVector StartLocation = AxisLoc + AxisX * 100.f;
	UKismetSystemLibrary::DrawDebugCoordinateSystem(this, StartLocation, AxisRot, 100.f, 5.f, 10.f);
	K2_EndAbility();
}