// 版权没有，拷贝自由。


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouseProxy(UGameplayAbility* OwningAbility)
{
	auto MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	// 如果本机控制，说明是由我们主动激活，在本机运行。
	if (IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		// Ability Task 只能在Owner 客户端和服务器运行。如果不是本机激活，则只可能是服务器。当然本机激活也可能存在是服务器的情况。
		// 综上，非本机控制激活，一定位于服务器。
		check(Ability->GetCurrentActorInfo()->IsNetAuthority());

		auto SpecHandle = GetAbilitySpecHandle();
		auto ActivationPKey = GetActivationPredictionKey();

		UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
		auto&& tdsDelegate = ASC->AbilityTargetDataSetDelegate(SpecHandle, ActivationPKey);
		tdsDelegate.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// 如果Target Data已经到达，那么我们就直接调用委托。
		const bool bCalledDelegate = ASC->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPKey);

		if (!bCalledDelegate) // 如果Target Data没有到达，那么我们就等待
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!ASC) return;

	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC) return;

	FScopedPredictionWindow PredictionWindow{ASC};


	auto Data = new FGameplayAbilityTargetData_SingleTargetHit();
	PC->GetHitResultUnderCursor(ECC_Visibility, false, Data->HitResult);
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	// 对于客户端，需要向服务器发送TargetData
	// 测试目的：退出Task，看看是否会导致TargetData丢失。
	if (!Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		ASC->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle,
		                                   FGameplayTag{}, ASC->ScopedPredictionKey
		);
	}

	// 如果Ability 没有位于激活状态，会阻止广播。因此需要以下检测。
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
	
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                           FGameplayTag GameplayTag)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),
	                                                                GetActivationPredictionKey());
}
