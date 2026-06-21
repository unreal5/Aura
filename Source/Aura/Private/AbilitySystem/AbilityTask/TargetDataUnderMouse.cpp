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
	const bool IsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (IsLocallyControlled) // 如果是本机控制，无论客户端/服务端都执行获取鼠标位置并发送数据的流程。
	{
		// SendMouseCursorData：1、向服务端发送数据，2、本地广播。
		/*
		 * 因此如果位于服务端并且本地控制也是可行的：
		 * 1、此时服务端不会监听发送数据的RPC，因此不会冗余处理
		 * 2、服务端本机控制也需要走获取鼠标位置并广播数据的流程，并且省掉了侦听->处理流程。
		 */
		SendMouseCursorData();
	}
	else // 一定位于服务端。GA只能在OwningClient和Server上执行。
	{
		// TODO: 测试：如果GA设置为“服务器优先”，则会触发以下断言，因为它不可能是本机控制的。
		check(Ability->GetCurrentActorInfo()->IsNetAuthority()); // 服务端执行时必须是NetAuthority。

		FPredictionKey OriginalPredictionKey = GetActivationPredictionKey();
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();

		auto&& Delegate = AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, OriginalPredictionKey);
		// 建立侦听
		Delegate.AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);
		// 检查是否已经收到了数据（如果是服务端触发的GA，可能之前就收到了数据了），如果收到了数据就让其再触发。
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, OriginalPredictionKey);
		if (!bCalledDelegate)
		{
			// 没有收到数据，说明数据还没有到达服务端，进入等待状态。
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow PredictionWindow(AbilitySystemComponent.Get());

	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC) return; // 调用本函数时已经进行过是否本机控制的判断了，这里只是保险。

	auto TargetData = new FGameplayAbilityTargetData_SingleTargetHit{};
	PC->GetHitResultUnderCursor(ECC_Visibility, false, TargetData->HitResult);
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(TargetData);
	// 客户端（服务器也适用）预测：对于预测数据，每次预测都会生成一个新的预测Key，服务端会根据这个Key来识别数据是否是预测数据，以及是否需要丢弃过时的预测数据。
	// Original PredictionKey：GA激活时生成的预测Key，服务端会将其与接收到的数据进行比较，如果数据的预测Key与Original PredictionKey匹配，则认为数据是有效的预测数据；如果不匹配，则认为数据是过时的预测数据，应该被丢弃。
	auto OriginalPredictionKey = GetActivationPredictionKey();
	FGameplayTag ApplicationTag{};
	FPredictionKey CurrentPredictionKey = AbilitySystemComponent->ScopedPredictionKey;
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), OriginalPredictionKey,
	                                                      TargetDataHandle, ApplicationTag, CurrentPredictionKey);

	// 这个函数承担了过多的职责。应该把获取鼠标位置和广播数据的流程分离出来。
	/* 	GAS 中 Ability 可能随时被取消（如角色死亡、技能被打断），如果此时 Task 仍在异步等待（如等待动画事件、等待目标数据），直接广播委托可能导致：
	- 访问已销毁的对象 → 崩溃
	- 在已结束的 Ability 上执行逻辑 → 状态混乱
	- 重复触发回调 → 逻辑错误

	这个函数相当于一道**安全闸门**，确保所有回调都在合法的生命周期内执行。
	*/
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReady.Broadcast(TargetDataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(
	const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTargetDataReady.Broadcast(GameplayAbilityTargetDataHandle);
	}
}
