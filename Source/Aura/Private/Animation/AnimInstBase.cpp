// 版权没有，拷贝自由。


#include "Animation/AnimInstBase.h"

#include "Character/AuraCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		GroundSpeed = CharacterMovement->Velocity.Size2D();
		bShouldMove = !FMath::IsNearlyZero(GroundSpeed);
	}
}
