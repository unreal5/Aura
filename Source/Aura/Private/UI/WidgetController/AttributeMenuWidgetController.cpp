// 版权没有，拷贝自由。


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Tag/GlobalTag.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	if (!AS) return;

	check(AttributeInfo);

	// Intelligence
	auto IntelligenceAttrInfo = AttributeInfo->FindAttributeInfoByTag(GlobalTag::Attributes_Primary_Intelligence);
	IntelligenceAttrInfo.AttributeValue = AS->GetIntelligence();
	AttributeInfoDelegate.Broadcast(IntelligenceAttrInfo);
	// Resilience
	auto ResilienceAttrInfo = AttributeInfo->FindAttributeInfoByTag(GlobalTag::Attributes_Primary_Resilience);
	ResilienceAttrInfo.AttributeValue = AS->GetResilience();
	AttributeInfoDelegate.Broadcast(ResilienceAttrInfo);
	// Strength
	auto StrengthAttrInfo = AttributeInfo->FindAttributeInfoByTag(GlobalTag::Attributes_Primary_Strength);
	StrengthAttrInfo.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(StrengthAttrInfo);
	// Vigor
	auto VigorAttrInfo = AttributeInfo->FindAttributeInfoByTag(GlobalTag::Attributes_Primary_Vigor);
	VigorAttrInfo.AttributeValue = AS->GetVigor();
	AttributeInfoDelegate.Broadcast(VigorAttrInfo);
	// Armor
	auto ArmorAttrInfo = AttributeInfo->FindAttributeInfoByTag(GlobalTag::Attributes_Secondary_Armor);
	ArmorAttrInfo.AttributeValue = AS->GetArmor();
	AttributeInfoDelegate.Broadcast(ArmorAttrInfo);
	// Armor Penetration
	auto ArmorPenetrationAttrInfo = AttributeInfo->FindAttributeInfoByTag(
		GlobalTag::Attributes_Secondary_ArmorPenetration);
	ArmorPenetrationAttrInfo.AttributeValue = AS->GetArmorPenetration();
	AttributeInfoDelegate.Broadcast(ArmorPenetrationAttrInfo);
	// Block Chance
	auto BlockChanceAttrInfo = AttributeInfo->FindAttributeInfoByTag(GlobalTag::Attributes_Secondary_BlockChance);
	BlockChanceAttrInfo.AttributeValue = AS->GetBlockChance();
	AttributeInfoDelegate.Broadcast(BlockChanceAttrInfo);
	// Critical Hit Chance
	auto CriticalHitChanceAttrInfo = AttributeInfo->FindAttributeInfoByTag(
		GlobalTag::Attributes_Secondary_CriticalHitChance);
	CriticalHitChanceAttrInfo.AttributeValue = AS->GetCriticalHitChance();
	AttributeInfoDelegate.Broadcast(CriticalHitChanceAttrInfo);
	// Critical Hit Damage
	auto CriticalHitDamageAttrInfo = AttributeInfo->FindAttributeInfoByTag(
		GlobalTag::Attributes_Secondary_CriticalHitDamage);
	CriticalHitDamageAttrInfo.AttributeValue = AS->GetCriticalHitDamage();
	AttributeInfoDelegate.Broadcast(CriticalHitDamageAttrInfo);
	// Critical Hit Resistance
	auto CriticalHitResistanceAttrInfo = AttributeInfo->FindAttributeInfoByTag(
		GlobalTag::Attributes_Secondary_CriticalHitResistance);
	CriticalHitResistanceAttrInfo.AttributeValue = AS->GetCriticalHitResistance();
	AttributeInfoDelegate.Broadcast(CriticalHitResistanceAttrInfo);
	// Health Regeneration
	auto HealthRegenerationAttrInfo = AttributeInfo->FindAttributeInfoByTag(
		GlobalTag::Attributes_Secondary_HealthRegeneration);
	HealthRegenerationAttrInfo.AttributeValue = AS->GetHealthRegeneration();
	AttributeInfoDelegate.Broadcast(HealthRegenerationAttrInfo);
	// Mana Regeneration
	auto ManaRegenerationAttrInfo = AttributeInfo->FindAttributeInfoByTag(
		GlobalTag::Attributes_Secondary_ManaRegeneration);
	ManaRegenerationAttrInfo.AttributeValue = AS->GetManaRegeneration();
	AttributeInfoDelegate.Broadcast(ManaRegenerationAttrInfo);
	// Max Health
	auto MaxHealthAttrInfo = AttributeInfo->FindAttributeInfoByTag(GlobalTag::Attributes_Secondary_MaxHealth);
	MaxHealthAttrInfo.AttributeValue = AS->GetMaxHealth();
	AttributeInfoDelegate.Broadcast(MaxHealthAttrInfo);
	// Max Mana
	auto MaxManaAttrInfo = AttributeInfo->FindAttributeInfoByTag(GlobalTag::Attributes_Secondary_MaxMana);
	MaxManaAttrInfo.AttributeValue = AS->GetMaxMana();
	AttributeInfoDelegate.Broadcast(MaxManaAttrInfo);
}
