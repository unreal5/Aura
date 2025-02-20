// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"

#include "Interaction/EnemyInterface.h"

#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
#pragma region IEnemyInterface
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;
#pragma endregion
	virtual void BeginPlay() override;
private:
	virtual void InitAbilityActorInfo() override;
};
