// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	void BeginPlay() override;

private:
	void HighlightActor_Implementation() override;
	void UnHighlightActor_Implementation() override;
};
