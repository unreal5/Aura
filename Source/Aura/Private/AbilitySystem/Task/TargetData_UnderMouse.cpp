// 版权没有，拷贝自由。


#include "AbilitySystem/Task/TargetData_UnderMouse.h"

#include "Player/AuraPlayerController.h"

UTargetData_UnderMouse* UTargetData_UnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetData_UnderMouse* MyObj = NewAbilityTask<UTargetData_UnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetData_UnderMouse::Activate()
{
	// Super::Activate();

	// task 知道它属于哪个 ability
	auto PC = Cast<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	if (PC)
	{
		FHitResult HitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		ValidData.Broadcast(HitResult.ImpactPoint);
	}
}
