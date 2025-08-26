// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

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
};
