// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class  UAnimMontage;

// This class does not need to be modified.
UINTERFACE(BlueprintType)
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Aura|战斗")
	int32 GetPlayerLevel() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Aura|战斗")
	FVector GetCombatSocketLocation() const;
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category="Aura|战斗")
	void UpdateFacingTarget(const FVector& TargetLocation);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="Aura|战斗")
	UAnimMontage* GetHitReactMontage() const;
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="Aura|战斗")
	void Die();
};