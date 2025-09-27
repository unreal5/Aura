// 版权没有，拷贝自由。


#include "Animation/AnimInstBase.h"

#include "Character/AuraCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void FAuraAnimInstanceProxy::SetMovementData(UCharacterMovementComponent* InCharMoveComp)
{
	CharacterMovementComp = InCharMoveComp;
}

void FAuraAnimInstanceProxy::Update(float DeltaSeconds)
{
	// 可在此基于已复制数据做一些派生计算（勿访问世界/Actor）。
	FAnimInstanceProxy::Update(DeltaSeconds);
	if (auto CharMoveComp = CharacterMovementComp.Get())
	{
		GroundSpeed = CharMoveComp->Velocity.Size2D();
		bShouldMove = !FMath::IsNearlyZero(GroundSpeed);
	}
}

void FAuraAnimInstanceProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	FAnimInstanceProxy::PostUpdate(InAnimInstance);
	if (auto AuraAnimInst = Cast<UAnimInstBase>(InAnimInstance))
	{
		AuraAnimInst->GroundSpeed = GroundSpeed;
		AuraAnimInst->bShouldMove = bShouldMove;
	}
}

void UAnimInstBase::NativeInitializeAnimation()
{
	AuraCharacter = Cast<AAuraCharacterBase>(GetOwningActor());
	if (AuraCharacter)
	{
		CharacterMovement = AuraCharacter->GetCharacterMovement();
	}
}

void UAnimInstBase::NativeUpdateAnimation(float DeltaSeconds)
{
	if (CharacterMovement)
	{
		// 线程安全复制
		GetAuraProxy().SetMovementData(CharacterMovement);
	}
}

FAuraAnimInstanceProxy& UAnimInstBase::GetAuraProxy()
{
	return GetProxyOnGameThread<FAuraAnimInstanceProxy>();
}
