// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHealth();

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	// Attribute to capture - Health
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vigor);
};
