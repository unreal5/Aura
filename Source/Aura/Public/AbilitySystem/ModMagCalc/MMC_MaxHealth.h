// 版本没有，拷贝自由。

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
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
private:
	// 宏 DECLARE_ATTRIBUTE_CAPTUREDEF 声明 VigorDef和VigorProperty
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vigor)
};
