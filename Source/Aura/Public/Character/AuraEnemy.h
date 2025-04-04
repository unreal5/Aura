// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"

#include "Interaction/EnemyInterface.h"

#include "AuraEnemy.generated.h"

class UWidgetComponent;
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
#pragma region IAbilitySystemInterface
	virtual int32 GetPlayerLevel() const override;
#pragma endregion
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
private:
	virtual void InitAbilityActorInfo() override;

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character class defaults",
		meta=(AllowPrivateAccess = "true"))
	int32 Level = 1;
};
