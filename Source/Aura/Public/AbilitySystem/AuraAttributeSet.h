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

	// Primary Attributes
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
	
	// Vital Attributes
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Health)


	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxHealth)


	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadOnly, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Mana)


	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category="Vital Attributes")
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
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const;
};

