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
	APlayerController* SourcePlayerController = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACharacter* SourceCharacter = nullptr;
	// Target info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerController* TargetPlayerController = nullptr;
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
	 *  Primary Attributes
	 */ 
	UPROPERTY(ReplicatedUsing=OnRep_Strength, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Strength)

	UPROPERTY(ReplicatedUsing=OnRep_Intelligence, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Intelligence)

	UPROPERTY(ReplicatedUsing=OnRep_Resilience, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Resilience)

	UPROPERTY(ReplicatedUsing=OnRep_Vigor, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Vigor)

	/*
	 * vital attributes
	 */ 
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Health)

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxHealth)

	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Mana)

	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxMana)

	// 
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
	
	/* Vital Attributes RepNotify */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	void GatherEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps) const;
};
