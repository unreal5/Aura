// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/CombatInterface.h"
#include "AuraPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	// 实现 IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// 结束 IAbilitySystemInterface
	// 重载 GetLifetimeReplicatedProps 来复制属性
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	int32 GetPlayerLevel() const { return Level; }
	int32 GetPlayerLevel_Implementation() const override { return GetPlayerLevel(); }
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;
private:
	UPROPERTY(ReplicatedUsing=OnRep_Level, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	int32 Level = 1;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};