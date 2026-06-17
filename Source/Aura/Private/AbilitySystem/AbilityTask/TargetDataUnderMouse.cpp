// 版权没有，拷贝自由。


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility,
                                                                         FName TaskInstanceName)
{
	return NewAbilityTask<UTargetDataUnderMouse>(OwningAbility, TaskInstanceName);
}

void UTargetDataUnderMouse::Activate()
{
	// Super::Activate();
	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC) return;

	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	// 这里为什么不判断没有命中？因为没有命中的话，HitResult.Location就是一个无效的值，广播出去也没什么关系。
	OnTargetDataReady.Broadcast(HitResult.Location);
}