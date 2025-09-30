// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "InterAction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	UPROPERTY(BlueprintAssignable)
	FOnDataChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDataChangedDelegate OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float BaskWalklSpeed = 250.f;

protected:
	virtual void BeginPlay() override;
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() const override { return Level; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Default", meta=(AllowPrivateAccess=true))
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Default", meta=(AllowPrivateAccess=true))
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};
