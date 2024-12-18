// 版本没有，拷贝自由。


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	// Super::Activate();

	/*
	 * 技能如果在客户端激活，由于技能是可以复制的，因此本地先激活，然后再由服务器激活。
	 * 服务器激活时，GetHitResultUnderCursor会返回ZeroVector，因此需要在客户端激活时获取。
	 */

	/*
	 *	GAS内置“向服务器发送/从服务器接收”方向
	 */

	// TODO:对于代码，其实可进一步简化：位于服务器，则按服务器流程(仍然需要判断IsLocalControlled，所以这种方法也可行)，
	// TODO:位于客户端，则肯定是Owning Client，即一定是本地控制。

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		// auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
		//UE_LOG(LogTemp, Warning, TEXT("%s"), PC->HasAuthority()?TEXT("服务器"):TEXT("客户端"));
		SendMouseCursorData();
	}
	else // 只能是服务器（对于Simulation来讲，不能激活技能）
	{
		FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		FPredictionKey PredictionKey = GetActivationPredictionKey();
		// 返回技能激活时的PredictionKey，只要预测键和技能句柄构成的键相同，就可以找到对应的技能。
		auto&& TargetDataSetDelegate = AbilitySystemComponent->AbilityTargetDataSetDelegate(
			AbilitySpecHandle, PredictionKey);
		TargetDataSetDelegate.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

	    // 服务器端，如果在设置前远程TargetData已经到达，因为没有消耗掉（回调函数中明确消耗掉处理的消息）
	    // 则调用CallReplicatedTargetDataDelegatesIfSet，重新播报消息；
	    // 否则（消息未到达），就设置等待远程玩家数据（阻塞）。
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
			AbilitySpecHandle, PredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	/**
	 *	非本地控制，针对GAS的技能环境来说，只能是服务器。
	 **/

	// 如果本机控制，要播报事件（ValidData）。
	// 如果本机控制，并位于服务器，要播报事件；
	// 如果本机控制，并位于客户端，要播报事件，并向服务器发送TargetData。

	// 创建Scoped预测窗口
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	// 由TargetDataHandle释放内存。
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetData->HitResult = CursorHit;

	// TargetDataHandle使用智能指针管理TargetData，不需要手动释放TargetData。
	FGameplayAbilityTargetDataHandle TargetDataHandle(TargetData);


	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(),
	                                                      TargetDataHandle, FGameplayTag{},
	                                                      AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                           FGameplayTag ActivationTag)
{
	// 消耗远程玩家数据（句柄、预测键组成的键值对）
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// 检查Ability的有效性，Ability存在并处于激活状态。
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
		//UE_LOG(LogTemp, Warning, TEXT("%s"), PC->HasAuthority()?TEXT("服务器"):TEXT("客户端"));
		// 经测试，只有在客户端激活技能时，才会在服务端触发此回调。
		ValidData.Broadcast(DataHandle);
	}
}
