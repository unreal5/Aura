// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

class ACharacter;

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	// Source info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AController* SourceController = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACharacter* SourceCharacter = nullptr;
	// Target info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AController* TargetController = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACharacter* TargetCharacter = nullptr;
	// Effect context
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayEffectContextHandle EffectContextHandle;
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	 *  Primary Attributes：不依赖其它属性的基础属性
	 */
	UPROPERTY(ReplicatedUsing=OnRep_Strength, BlueprintReadOnly, Category = "Primary Attributes",
		meta=(ToolTip = "增加物理伤害"))
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Strength)

	UPROPERTY(ReplicatedUsing=OnRep_Intelligence, BlueprintReadOnly, Category = "Primary Attributes",
		meta=(ToolTip = "增加法术伤害"))
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Intelligence)

	UPROPERTY(ReplicatedUsing=OnRep_Resilience, BlueprintReadOnly, Category = "Primary Attributes",
		meta=(ToolTip = "增加韧性，加强护甲及穿透"))
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Resilience)

	UPROPERTY(ReplicatedUsing=OnRep_Vigor, BlueprintReadOnly, Category = "Primary Attributes",
		meta=(ToolTip = "增加活力，增加生命恢复速度"))
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Vigor)

	/* 
	 * Secondary Attributes：依赖于Primary Attributes的属性，不能直接修改，只能通过修改Primary Attributes来间接修改
	 */
	UPROPERTY(ReplicatedUsing=OnRep_Armor, BlueprintReadOnly, Category = "Secondary Attributes",
		meta=(ToolTip = "减伤，增加格挡几率。依赖于韧性"))
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Armor)


	UPROPERTY(ReplicatedUsing=OnRep_ArmorPenetration, BlueprintReadOnly, Category = "Secondary Attributes",
		meta=(ToolTip = "忽略敌方护甲的几率，增加暴击伤害几率。依赖于韧性"))
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, ArmorPenetration)

	UPROPERTY(ReplicatedUsing=OnRep_BlockChance, BlueprintReadOnly, Category = "Secondary Attributes",
		meta=(ToolTip = "增加格挡几率，格挡成功后伤害减半。依赖于Armor"))
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, BlockChance)

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitChance, BlueprintReadOnly, Category = "Secondary Attributes",
		meta=(ToolTip = "增加暴击几率，暴击成功后，伤害加倍。依赖于ArmorPenetration"))
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitChance)

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitDamage, BlueprintReadOnly, Category = "Secondary Attributes",
		meta=(ToolTip = "暴击时，额外增加的伤害。依赖于ArmorPenetration"))
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitDamage)

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitResistance, BlueprintReadOnly, Category = "Secondary Attributes",
		meta=(ToolTip = "减少敌方暴击几率。依赖于Armor"))
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitResistance)

	UPROPERTY(ReplicatedUsing=OnRep_HealthRegeneration, BlueprintReadOnly, Category = "Secondary Attributes",
		meta=(ToolTip = "增加生命恢复速度。依赖于活力"))
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, HealthRegeneration)

	UPROPERTY(ReplicatedUsing=OnRep_ManaRegeneration, BlueprintReadOnly, Category = "Secondary Attributes",
		meta=(ToolTip = "增加法力恢复速度。依赖于智力"))
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, ManaRegeneration)

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category = "Vital Attributes",
		meta=(ToolTip = "生命上限，当前生命不能超过这个值。依赖于活力。"))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxHealth)

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category = "Vital Attributes",
		meta=(ToolTip = "法力上限，当前法力不能超过这个值。依赖于智力。"))
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxMana)
	/*
	 * vital attributes
	 */
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Health)

	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Mana)

	// Meta-Attributes：元属性，不可复制，可以作为PlaceHolder
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, IncomingDamage)
	
	// 重载
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	/* Primary Attributes RepNotify */
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	/* Secondary Attributes RepNotify */
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	/* Vital Attributes RepNotify */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;


	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;


	void GatherEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps) const;
	
	void ShowFloatingText(const FEffectProperties& EffectProperties, float Damage) const;
};