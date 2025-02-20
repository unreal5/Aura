// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"

#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "技能组件")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "技能组件")
	TObjectPtr<UAttributeSet> AttributeSet;
};
