// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "AnimInstBase.generated.h"

class AAuraCharacterBase;
class UCharacterMovementComponent;
/**
 * 
 */
struct FAuraAnimInstanceProxy : public FAnimInstanceProxy
{
	FAuraAnimInstanceProxy(UAnimInstance* Instance) : FAnimInstanceProxy(Instance)
	{
	}

	// 线程安全副本
	float GroundSpeed = 0.f;
	bool bShouldMove = false;

	void SetMovementData(UCharacterMovementComponent* InCharMoveComp);

	virtual void Update(float DeltaSeconds) override;

protected:
	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;

private:
	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComp;


	// 如需自定义姿势求值：
	// virtual void Evaluate(FPoseContext& Output) override { ... }
};

UCLASS()
class AURA_API UAnimInstBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TObjectPtr<AAuraCharacterBase> AuraCharacter;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation")
	float GroundSpeed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Animation")
	bool bShouldMove;

	// 创建与销毁 Proxy
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return new FAuraAnimInstanceProxy(this);
	}

	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override
	{
		delete InProxy;
	}

	FAuraAnimInstanceProxy& GetAuraProxy();
	friend FAuraAnimInstanceProxy;
};
