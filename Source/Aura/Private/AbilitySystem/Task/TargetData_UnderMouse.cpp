// 版权没有，拷贝自由。


#include "AbilitySystem/Task/TargetData_UnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetData_UnderMouse* UTargetData_UnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetData_UnderMouse* MyObj = NewAbilityTask<UTargetData_UnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetData_UnderMouse::Activate()
{
	// Super::Activate();
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		//TODO: on server, listen for target data
	}
	
}

void UTargetData_UnderMouse::SendMouseCursorData()
{
	// 如果本机做一些需要预测的操作，必须有预测窗口。
	// Scoped表示这个预测窗口在这个作用域内有效，出了这个作用域就无效了。
	// 这个预测窗口的作用是：在这个窗口内，所有对AbilitySystemComponent的修改，都会被认为是预测的。
	// 预测的结果会被发送到服务器，服务器会验证这些预测的结果。
	// 如果服务器验证通过，那么这些预测的结果就会被应用到服务器上。
	// 如果服务器验证不通过，那么这些预测的结果就会被丢弃。
	// 这个预测窗口的生命周期是：当这个对象被销毁的时候，预测窗口也会被销毁。
	// 所以，这个预测窗口的生命周期是：当这个函数执行完毕的时候，预测窗口就会被销毁。
	// 这样做的好处是：可以确保预测窗口的生命周期是有限的，不会无限制地延长。
	// 这样做的坏处是：如果在这个函数执行完毕之前，有其他的代码需要使用预测窗口，那么这些代码就无法使用预测窗口了。
	// 所以，在使用预测窗口的时候，一定要确保在这个函数执行完毕之前，不会有其他的代码需要使用预测窗口。
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	auto Data = new FGameplayAbilityTargetData_SingleTargetHit;
	Data->HitResult = HitResult;
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(Data);
	auto CurrentPredictionKey = AbilitySystemComponent->ScopedPredictionKey;
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(),
	                                                      TargetDataHandle, FGameplayTag(), CurrentPredictionKey);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}
