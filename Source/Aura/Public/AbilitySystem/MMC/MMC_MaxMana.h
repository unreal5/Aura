// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	// Attribute to capture - Mana
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
};
