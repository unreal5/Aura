// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Engine/HitResult.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UNiagaraSystem;
class UAuraAbilitySystemComponent;
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
	void TryHighlightActor(AActor* Actor);
	void TryUnHighlightActor(AActor* Actor);
	FHitResult CursorHit;
	// 开始地面点击移动
	// 左键点击地面效果
	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, Category="自动移动")
	TObjectPtr<UNiagaraSystem> FXCursor;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, Category="自动移动", meta=(ShortTooltip="小于此阈值的点击被认为是短按", DisplayName = "短按阈值"))
	float ShortPressThreshold = 0.2f;

	FVector CachedDestination = FVector::ZeroVector;
	/** Time that the click input has been pressed */
	float FollowTime = 0.0f;
	bool bAutoRunning = false;
	// 光标下面是否是敌人。
	bool bTargeting = false;
	
	UPROPERTY(EditAnywhere, Category="自动移动")
	float AcceptanceRadius = 50.0f;
	
	UPROPERTY(VisibleAnywhere, Category="自动移动")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;
	
	void AutoRun();
	// 结束地面点击移动


	void ToggleAttributeDebugPanel();
	UPROPERTY(Transient)
	TObjectPtr<AActor> LastActor = nullptr;
	UPROPERTY(Transient)
	TObjectPtr<AActor> CurrentActor = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UAuraAbilitySystemComponent> CachedAuraASC = nullptr;

	UAuraAbilitySystemComponent* GetASC();

	void AbilityInputPressed(const FGameplayTag InputTag);
	void AbilityInputReleased(const FGameplayTag InputTag);
	void AbilityInputHeld(const FGameplayTag InputTag);
};