// 版权没有，拷贝自由。


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "InterAction/CombatInterface.h"


UMMC_MaxHealth::UMMC_MaxHealth()
{
	// 当前应用效果时，源和目标都是同一Character，因此随意选择即可。
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Vigor, Source, false);

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	Params.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, Params, Vigor);
	Vigor = FMath::Max(0.f, Vigor); // 确保Vigor不为负值

	UObject* SrcObject = Spec.GetContext().GetSourceObject();
	const int32 PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SrcObject);

	const auto CalculatedMaxHealth = 80.f + Vigor * 2.5f + PlayerLevel * 10.f;
	return CalculatedMaxHealth;
}
