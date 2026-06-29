// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraCommonTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category="Aura | Delegate")
	FOnAttributeValueChangedWithTagSignature OnAttributeValueChangedWithTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="战斗")
	float Lifespan = 5.f;
	
	/* 实现ICombatInterface*/
	int32 GetPlayerLevel_Implementation() const override;
	void Die_Implementation() override;
	
	void HighlightActor_Implementation() override;
	void UnHighlightActor_Implementation() override;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="战斗")
	bool bHitReacting = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="移动")
	float BaseWalkSpeed = 250.f;
protected:
	void InitAbilityActorInfo() override;
	void InitializeDefaultAttributes() const override;
private:


	//对于敌人，数据只存在于服务器上，客户端不需要知道，所以不复制。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="角色默认", meta=(AllowPrivateAccess=true))
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="角色默认", meta=(AllowPrivateAccess=true))
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	//血条组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="组件", meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> HealthBarWidgetComponent;
	

};