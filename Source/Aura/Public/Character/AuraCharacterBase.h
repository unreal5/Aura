// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"

#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "技能组件")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "技能组件")
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo() {}
};
