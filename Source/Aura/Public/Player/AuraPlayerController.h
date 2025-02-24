// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

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

	void Move(const FInputActionValue& Value);

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* CurrentActor;
};
