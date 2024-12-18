// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual int32 GetPlayerLevel() const override;

private:
	virtual void InitAbilityActorInfo() override;
};
