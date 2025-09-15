// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() const override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	TObjectPtr<class UCameraComponent> ViewCamera;

	virtual void InitAbilityActorInfo() override;
};
