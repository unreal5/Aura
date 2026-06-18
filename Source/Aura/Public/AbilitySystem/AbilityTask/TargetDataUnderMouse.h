// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "Target Data Under Mouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "TRUE"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility,
	                                                         FName TaskInstanceName = FName("TargetDataUnderMouse"));

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataDelegate OnTargetDataReady;

protected:
	void Activate() override;
	
private:
	void SendMouseCursorData();
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& GameplayAbilityTargetDataHandle, FGameplayTag GameplayTag);
};