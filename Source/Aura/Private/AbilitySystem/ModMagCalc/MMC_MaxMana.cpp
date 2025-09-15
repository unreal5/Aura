// 版权没有，拷贝自由。


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "InterAction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana() {
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Intelligence, Source, false);
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	Params.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, Params, Intelligence);
	Intelligence = FMath::Max(0.f, Intelligence); // 确保Intelligence不为负值

	UObject* SrcObject = Spec.GetContext().GetSourceObject();
	const int32 PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SrcObject);
	const auto CalculatedMaxMana = 50.f + Intelligence * 2.5f + PlayerLevel * 15.f;
	return CalculatedMaxMana;
}
