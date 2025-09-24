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
	// const bool bIsServer = Ability->GetCurrentActorInfo()->IsNetAuthority();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		// 如果数据先到达，就直接广播（需要找出这个数据）
		// 如果数据后到达，就缓存数据，等数据到达后再广播
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey OriginalPredictionKey = GetActivationPredictionKey();

		auto&& TargetDataSetDelegate = AbilitySystemComponent->AbilityTargetDataSetDelegate(
			SpecHandle, OriginalPredictionKey);
		TargetDataSetDelegate.AddLambda([this](const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag InTag)
		{
			// 数据到达，使用这个数据，并使其无效。
			FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
			FPredictionKey OriginalPredictionKey = GetActivationPredictionKey();
			AbilitySystemComponent->ConsumeClientReplicatedTargetData(SpecHandle, OriginalPredictionKey);

			// 这里不需要移除委托，因为这个委托是一次性的，数据到达后就会被移除。
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				ValidData.Broadcast(DataHandle);
			}
		});
		// 查找数据是否已经到达，如果已经到达，就直接广播
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
			SpecHandle, OriginalPredictionKey);
		// bCalledDelegate 表示是否已经调用了委托，如果已经调用了，就不需要再等待数据到达了，否则就需要等待数据到达。
		// 这是异步Task，不会阻塞主线程。
		if (false == bCalledDelegate)
		{
			// 等待数据到达
			// 这里不需要做任何事情，因为上面的委托已经添加了，当数据到达时会自动调用委托。
			// 只需要阻塞当前函数的执行，直到数据到达为止。
			SetWaitingOnRemotePlayerData();
		}
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
	auto Data = new FGameplayAbilityTargetData_SingleTargetHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, Data->HitResult);
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	// Add 函数会将指针包装成 TSharedPtr，因此不用delete
	TargetDataHandle.Add(Data);
	auto CurrentPredictionKey = AbilitySystemComponent->ScopedPredictionKey;
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(),
	                                                      TargetDataHandle, FGameplayTag(), CurrentPredictionKey);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}
