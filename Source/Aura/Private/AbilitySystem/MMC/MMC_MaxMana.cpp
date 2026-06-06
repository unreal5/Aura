// 版权没有，拷贝自由。


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Intelligence, Target, false);

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	auto SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	auto TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Intelligence Value
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max(Intelligence, 0.f);

	// Get Player Level  
	UObject* SourceObject = Spec.GetContext().GetEffectCauser();
	const int32 PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceObject);
	// 计算最大法力值
	return 50.f + Intelligence * 2.5f + PlayerLevel * 15.f;
}
