// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

struct FInputActionValue;
class UAuraInputConfig;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="输入")
	TObjectPtr<class UInputMappingContext> AuraContext;

	UPROPERTY(EditDefaultsOnly, Category="输入")
	TObjectPtr<class UInputAction> MoveAction;


	void CursorTrace();

	UPROPERTY()
	AActor* LastActor = nullptr;
	UPROPERTY()
	AActor* ThisActor = nullptr;

	// 输入回调函数
	void AbilityInputTagPressed(const FInputActionValue& Value, FGameplayTag InputTag);
	void AbilityInputTagReleased(const FInputActionValue& Value, FGameplayTag InputTag);
	void AbilityInputTagHeld(const FInputActionValue& Value, FGameplayTag InputTag);
	// 输入配置
	UPROPERTY(EditDefaultsOnly, Category="输入")
	TObjectPtr<UAuraInputConfig> InputConfig;
};
