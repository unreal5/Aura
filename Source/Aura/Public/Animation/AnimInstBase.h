// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAnimInstBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TObjectPtr<class AAuraCharacterBase> AuraCharacter;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TObjectPtr<class UCharacterMovementComponent> CharacterMovement;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation")
	float GroundSpeed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation")
	bool bShouldMove;
};
