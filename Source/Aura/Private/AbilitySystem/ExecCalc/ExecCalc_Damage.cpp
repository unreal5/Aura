// 版本没有，拷贝自由。


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

namespace
{
	struct AuraDamageStatics
	{
		DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
		DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)

		DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)

		DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
		DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
		DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)

		DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
		DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
		DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
		DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

		TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

		AuraDamageStatics()
		{
			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false)

			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)

			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false)

			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false)

			// 将所有的TagsToCaptureDefs添加到TagsToCaptureDefs中。
			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_Armor, ArmorDef);
			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);

			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_BlockChance, BlockChanceDef);

			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);

			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Resistance_Fire, FireResistanceDef);
			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Resistance_Lightning, LightningResistanceDef);
			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Resistance_Arcane, ArcaneResistanceDef);
			TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Resistance_Physical, PhysicalResistanceDef);
			
		}
	};

	static const AuraDamageStatics DamageStatics()
	{
		static AuraDamageStatics DStatics;
		return DStatics;
	}
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);

	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	auto SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	auto TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	auto& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 也可以使用"set by caller magnitude"来获取伤害值
	// SetByCallerMagnitude是一个Map，可以存储多个值，通过Key来获取，参考 "AuraProjectileSpell.cpp" 中的设置。
	// Note: SetByCallerMagnitude 不是函数，只是一个 TMap<FGameplayTag, float> 类型的变量。
	float Damage = 0.f;
	for (const auto& [Key,Value] : AuraGameplayTags::DamageTypesToResistance)
	{
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Key, false, 0.f);
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = AuraDamageStatics().TagsToCaptureDefs[Value];
		float ResistanceValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvaluationParameters,
		                                                           ResistanceValue);
		
		ResistanceValue= FMath::Clamp(ResistanceValue,0.f, 100.f);
		// 伤害减少
		DamageTypeValue *= (100.f - ResistanceValue) / 100.f;
		
		Damage += DamageTypeValue;
	}


	// Captuer BlockChance on Target, and determine if the target blocks the damage successfully.
	// if block, half the damage.
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters,
	                                                           TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	// “Chance”，表示概率。这里是一个随机数，如果随机数小于BlockChance，那么就会触发Block。
	const bool bBlocked = FMath::RandRange(1.f, 100.f) <= TargetBlockChance;
	// 如果被Block了，伤害减半。
	if (bBlocked) Damage *= 0.5f;

	auto EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);


	// Captuer Armor on Target, and ArmorPenetration on Source.
	// ArmorPenetration ignores a percentage of the target's armor.
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters,
	                                                           TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,
	                                                           EvaluationParameters,
	                                                           SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	// CharacterClassInfo
	auto CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	auto ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("ArmorPenetration"), FString{});
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	// 等效护甲
	const float EffectiveArmor = TargetArmor * (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("EffectiveArmor"), FString{});
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	// 计算暴击(Critical Hit)
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,
	                                                           EvaluationParameters,
	                                                           SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(0.f, SourceCriticalHitChance);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,
	                                                           EvaluationParameters,
	                                                           TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(0.f, TargetCriticalHitResistance);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,
	                                                           EvaluationParameters,
	                                                           SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(0.f, SourceCriticalHitDamage);

	// TargetCriticalHitResistance * 因子 ，表示暴击抵抗的效果。
	FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("CriticalHitResistance"), FString{});
	float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	CriticalHitResistanceCoefficient = FMath::Max(0.f, CriticalHitResistanceCoefficient);
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance *
		CriticalHitResistanceCoefficient;

	const bool bCriticalHit = FMath::RandRange(1.f, 100.f) <= EffectiveCriticalHitChance;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	// 如果暴击，伤害翻倍。
	if (bCriticalHit)
	{
		Damage = 2.f * Damage + SourceCriticalHitDamage;
	}

	FGameplayModifierEvaluatedData EvaluatedData{
		UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage
	};
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}