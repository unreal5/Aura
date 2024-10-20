// 版本没有，拷贝自由。


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"

#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Intelligence, Target, false)

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	
	check(CombatInterface);

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max(0.f, Intelligence);
	
	return 50.f+ 2.5f * Intelligence + 15.f * PlayerLevel;
}
