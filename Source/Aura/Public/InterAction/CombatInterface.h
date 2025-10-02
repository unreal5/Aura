// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel() const;

	UFUNCTION(BlueprintNativeEvent)
	FVector GetCombatSocketLocation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetHitReactMontage() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Die();
};
