// 版本没有，拷贝自由。


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"

#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// 宏 DEFINE_ATTRIBUTE_CAPTUREDEF 初始化 VigorDef和VigorProperty

	// bSnapshot 决定何时捕获属性。例如在创建火球后，经过一段时间，命中目标：
	// 如bSnapshot为true,则应用创建GameplayEffect时的属性，false则使用GameplayEffect生效时（命中目标）的属性。
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Vigor, Target, false);

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from the source and target 
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 以下SourceTags和TargetTags是必须步骤
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max(Vigor, 0.f);

	// 这里为了学习，添加依赖PlayerLevel的计算
	float PlayerLevel = 0.f;
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject()))
	{
		PlayerLevel = CombatInterface->GetPlayerLevel();
	}

	// vigor = 9.f, PlayerLevel = 1.f, return 80.f + 2.5f * 9.f + 10.f * 1.f = 112.f;
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
