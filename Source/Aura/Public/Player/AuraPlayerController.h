// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	void PlayerTick(float DeltaTime) override;

protected:
	void BeginPlay() override;
	void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> AuraInputConfig;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* DebugAction;
		
	// 追踪当前光标下面的物体
	void CursorTrace();
	void ToggleAttributeDebugPanel();
	UPROPERTY(Transient)
	TObjectPtr<AActor> LastActor = nullptr;
	UPROPERTY(Transient)
	TObjectPtr<AActor> CurrentActor = nullptr;
	
	void AbilityInputPressed(const FGameplayTag InputTag);
	void AbilityInputReleased(const FGameplayTag InputTag);
	void AbilityInputHeld(const FGameplayTag InputTag);
};
