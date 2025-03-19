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
	// Super::Activate();

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		auto SpecHandle = GetAbilitySpecHandle();
		FPredictionKey PredictionKey = GetActivationPredictionKey();

		auto&& TargetDataSetDelegate = AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			SpecHandle, PredictionKey);
		TargetDataSetDelegate.AddLambda(
			[this, PredictionKey](const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag GameplayTag)
			{
				// 不用再保存DataHandle
				AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), PredictionKey);
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					ValidData.Broadcast(DataHandle);
				}

				// bool bHasAuthority = AbilitySystemComponent->GetAvatarActor()->HasAuthority();
				// FString Msg =bHasAuthority?TEXT("Server"):TEXT("Client");
				// UE_LOG(LogTemp, Warning,TEXT("%s"), *Msg );
			});
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(
			SpecHandle, PredictionKey);
		// 如果Target Data没有到达，那么我们就等待
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC) return;

	FScopedPredictionWindow PredictionWindow{AbilitySystemComponent.Get()};

	auto Data = new FGameplayAbilityTargetData_SingleTargetHit();
	PC->GetHitResultUnderCursor(ECC_Visibility, false, Data->HitResult);
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(),
	                                                      DataHandle, FGameplayTag{},
	                                                      AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
