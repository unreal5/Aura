// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

struct FGameplayEffectModCallbackData;

USTRUCT(BlueprintType, Blueprintable)
struct FEffectProperties
{
	GENERATED_BODY()

	FGameplayEffectContextHandle EffectContextHandle;
	// Source = causer of the effect, Target = target of the effect(owner of this AS)

	// Source
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect Properties")
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect Properties")
	TObjectPtr<AActor> SourceAvatarActor = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect Properties")
	TObjectPtr<APlayerController> SourcePlayerController = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect Properties")
	TObjectPtr<ACharacter> SourceCharacter = nullptr;

	// Target
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect Properties")
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect Properties")
	TObjectPtr<AActor> TargetAvatarActor = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect Properties")
	TObjectPtr<APlayerController> TargetPlayerController = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect Properties")
	TObjectPtr<ACharacter> TargetCharacter = nullptr;
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributesMap;
	
	// Primary Attributes --- 主属性不依赖任何其他属性
	UPROPERTY(ReplicatedUsing=OnRep_Strength, BlueprintReadOnly, Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Strength)

	UPROPERTY(ReplicatedUsing=OnRep_Intelligence, BlueprintReadOnly, Category="Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Intelligence)


	UPROPERTY(ReplicatedUsing=OnRep_Resilience, BlueprintReadOnly, Category="Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Resilience)

	UPROPERTY(ReplicatedUsing=OnRep_Vigor, BlueprintReadOnly, Category="Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Vigor)

	// Vital Attributes --- 生命值、法力值等
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Health)

	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadOnly, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Mana)

	// Secondary Attributes --- 次属性依赖主属性计算得出
	UPROPERTY(ReplicatedUsing=OnRep_Armor, BlueprintReadOnly, Category="Secondary Attributes",
		meta=(ToolTip = "减少受到的伤害，增加格挡几率"))
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Armor)

	UPROPERTY(ReplicatedUsing=OnRep_ArmorPenetration, BlueprintReadOnly, Category="Secondary Attributes",
		meta=(ToolTip = "无视目标护甲值的百分比，增加致命一击的机率"))
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, ArmorPenetration)

	UPROPERTY(ReplicatedUsing=OnRep_BlockChance, BlueprintReadOnly, Category="Secondary Attributes",
		meta=(ToolTip = "格挡几率，成功后伤害减半"))
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, BlockChance)

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitChance, BlueprintReadOnly, Category="Secondary Attributes",
		meta=(ToolTip = "致命一击的机率，成功后伤害翻倍"))
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitChance)

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitDamage, BlueprintReadOnly, Category="Secondary Attributes",
		meta=(ToolTip = "致命一击时的伤害加成"))
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitDamage)

	UPROPERTY(ReplicatedUsing=OnRep_CriticalHitResistance, BlueprintReadOnly, Category="Secondary Attributes",
		meta=(ToolTip = "减少被致命一击的机率"))
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, CriticalHitResistance)

	UPROPERTY(ReplicatedUsing=OnRep_HealthRegeneration, BlueprintReadOnly, Category="Secondary Attributes",
		meta=(ToolTip = "每秒回血量"))
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, HealthRegeneration)

	UPROPERTY(ReplicatedUsing=OnRep_ManaRegeneration, BlueprintReadOnly, Category="Secondary Attributes",
		meta=(ToolTip = "每秒法力回复量"))
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, ManaRegeneration)

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category="Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxHealth)

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category="Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxMana)

private:
	// RepNotify functions for attributes
	// Primary RepNotify functions
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	// Vital RepNotify functions
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	// Secondary RepNotify functions
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

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const;
};
