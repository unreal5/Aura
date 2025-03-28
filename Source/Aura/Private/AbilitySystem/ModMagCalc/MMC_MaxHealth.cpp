// 版权没有，拷贝自由。


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	// FGameplayEffectAttributeCaptureDefinition 构造函数
	// bSnapshot 决定何时捕获属性值，创建时还是应用时？
	// 对于当前情况，两者先后发生，创建后立即应用，因此一样。
	// 如果创建后，经过若干时间后应用，两者可能不一致。
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Vigor, Target, false);

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(0.f, Vigor);
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10 * PlayerLevel;
}
