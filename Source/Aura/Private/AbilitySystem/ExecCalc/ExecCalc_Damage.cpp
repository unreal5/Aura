// 版权没有，拷贝自由。


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Tag/GlobalTag.h"

// 辅助结构体
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	AuraDamageStatics();
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DmgStatics;
	return DmgStatics;
}

AuraDamageStatics::AuraDamageStatics()
{
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	auto SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	auto TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	auto&& Spec = ExecutionParams.GetOwningSpec();
	// 获取 SetByCaller 伤害数值。SetByCaller是在GameplayEffectSpec上设置的
	// 回想一下 AuraProjectileSpell 里是如何设置的。
	// 原始实现方案是在GE中修改InComingDamage元数据。
	// 此处展示了另一种使用SetByCaller的方式。
	float Damage = Spec.GetSetByCallerMagnitude(GlobalTag::Damage);

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();


	// ExecutionCalculate 不会执行PreAttribute/PreBaseAttribute clamp操作，如果需要请自行处理
	// 伤害数据保存在元数据InComingDamage中，这样最后统一在UAuraAttributeSet::PostGameplayEffectExecute中处理

	// 捕获目标的BlockChance（格挡几率），成功后伤害减半
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvalParams,
	                                                           TargetBlockChance);
	TargetBlockChance = FMath::Max(TargetBlockChance, 0.f);
	// 生成1~100之间的随机数
	if (FMath::RandRange(1, 100) <= TargetBlockChance) // 格挡成功，伤害减半
	{
		Damage *= 0.5f;
	}
	// 捕获目标的Armor（护甲值）
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvalParams, TargetArmor);
	TargetArmor = FMath::Max(TargetArmor, 0.f);
	// 捕获来源的ArmorPenetration（护甲穿透）
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvalParams,
	                                                           SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.f);
	// 计算有效护甲值：ArmorPenetration值表示忽略目标Armor的百分比。例如穿透是0.3，表示忽略30%的护甲值
	const float EffectiveArmor = TargetArmor *= (100.f - SourceArmorPenetration * 0.25) / 100.f;
	Damage *= (100.f - EffectiveArmor * 0.333f) / 100.f;

	// 目标的Armor（护甲值）会减少伤害
	FGameplayModifierEvaluatedData ModifierData{
		UAuraAttributeSet::GetInComingDamageAttribute(), EGameplayModOp::Additive, Damage
	};
	OutExecutionOutput.AddOutputModifier(ModifierData);
}
