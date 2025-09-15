// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "InterAction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void InitAbilityActorInfo() override;
	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() const override { return Level; }
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Default", meta=(AllowPrivateAccess=true))
	int32 Level = 1;
};
