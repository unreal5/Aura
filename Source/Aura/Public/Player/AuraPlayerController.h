// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IEnemyInterface;
class UInputAction;
struct FInputActionValue;

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	void AutoRun();
	// 只有本机控制玩家才会调用PlayerTick
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed();
	void ShiftReleased();

	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& Value);

	FHitResult CursorHit;
	void CursorTrace();

	IEnemyInterface* LastActor;
	IEnemyInterface* CurrentActor;

	// 为什么不能绑定到常量引用？
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;

	float FollowTime = 0.f;

	UPROPERTY(EditDefaultsOnly, meta=(ToolTip = "长按时间阈值"))
	float ShortPreesThreshold = 0.5f;

	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> SplineComponent;
};
