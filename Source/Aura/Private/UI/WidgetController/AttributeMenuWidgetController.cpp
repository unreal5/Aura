// 版权没有，拷贝自由。


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Tag/AuraGlobalTags.h"


void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	// 主属性：和AuraAttributeSet 中一致
	auto Strength = AttributeInfo->GetAttributeInfoByTag(Attributes::Primary::Strength, true);
	Strength.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Strength);
	
	auto Intelligence = AttributeInfo->GetAttributeInfoByTag(Attributes::Primary::Intelligence, true);
	Intelligence.AttributeValue = AS->GetIntelligence();
	AttributeInfoDelegate.Broadcast(Intelligence);
	
	auto Resilience = AttributeInfo->GetAttributeInfoByTag(Attributes::Primary::Resilience, true);
	Resilience.AttributeValue = AS->GetResilience();
	AttributeInfoDelegate.Broadcast(Resilience);
	
	auto Vigor = AttributeInfo->GetAttributeInfoByTag(Attributes::Primary::Vigor, true);
	Vigor.AttributeValue = AS->GetVigor();
	AttributeInfoDelegate.Broadcast(Vigor);
	
	// 次属性：和AuraAttributeSet 中一致
	auto Armor = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::Armor, true);
	Armor.AttributeValue = AS->GetArmor();
	AttributeInfoDelegate.Broadcast(Armor);
	
	auto ArmorPenetration = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::ArmorPenetration, true);
	ArmorPenetration.AttributeValue = AS->GetArmorPenetration();
	AttributeInfoDelegate.Broadcast(ArmorPenetration);
	
	auto BlockChance = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::BlockChance, true);
	BlockChance.AttributeValue = AS->GetBlockChance();
	AttributeInfoDelegate.Broadcast(BlockChance);
	
	auto CriticalHitChance = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::CriticalHitChance, true);
	CriticalHitChance.AttributeValue = AS->GetCriticalHitChance();
	AttributeInfoDelegate.Broadcast(CriticalHitChance);
	
	auto CriticalHitDamage = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::CriticalHitDamage, true);
	CriticalHitDamage.AttributeValue = AS->GetCriticalHitDamage();
	AttributeInfoDelegate.Broadcast(CriticalHitDamage);
	
	auto CriticalHitResistance = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::CriticalHitResistance, true);
	CriticalHitResistance.AttributeValue = AS->GetCriticalHitResistance();
	AttributeInfoDelegate.Broadcast(CriticalHitResistance);
	
	auto HealthRegeneration = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::HealthRegeneration, true);
	HealthRegeneration.AttributeValue = AS->GetHealthRegeneration();
	AttributeInfoDelegate.Broadcast(HealthRegeneration);
	
	auto ManaRegeneration = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::ManaRegeneration, true);
	ManaRegeneration.AttributeValue = AS->GetManaRegeneration();
	AttributeInfoDelegate.Broadcast(ManaRegeneration);
	
	auto MaxHealth = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::MaxHealth, true);
	MaxHealth.AttributeValue = AS->GetMaxHealth();
	AttributeInfoDelegate.Broadcast(MaxHealth);
	
	auto MaxMana = AttributeInfo->GetAttributeInfoByTag(Attributes::Secondary::MaxMana, true);
	MaxMana.AttributeValue = AS->GetMaxMana();
	AttributeInfoDelegate.Broadcast(MaxMana);
}


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent) return;
	if (!AttributeSet) return;
	
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	auto Lambda = [this](const FOnAttributeChangeData& Data, const FGameplayTag& AttributeTag)
	{
		auto AttributeInfo = this->AttributeInfo->GetAttributeInfoByTag(AttributeTag, false);
		
		AttributeInfo.AttributeValue = Data.NewValue;
		AttributeInfoDelegate.Broadcast(AttributeInfo);
	};
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetStrengthAttribute()).AddLambda(Lambda, Attributes::Primary::Strength.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetIntelligenceAttribute()).AddLambda(Lambda, Attributes::Primary::Intelligence.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetResilienceAttribute()).AddLambda(Lambda, Attributes::Primary::Resilience.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetVigorAttribute()).AddLambda(Lambda, Attributes::Primary::Vigor.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetArmorAttribute()).AddLambda(Lambda, Attributes::Secondary::Armor.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetArmorPenetrationAttribute()).AddLambda(Lambda, Attributes::Secondary::ArmorPenetration.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetBlockChanceAttribute()).AddLambda(Lambda, Attributes::Secondary::BlockChance.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetCriticalHitChanceAttribute()).AddLambda(Lambda, Attributes::Secondary::CriticalHitChance.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetCriticalHitDamageAttribute()).AddLambda(Lambda, Attributes::Secondary::CriticalHitDamage.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetCriticalHitResistanceAttribute()).AddLambda(Lambda, Attributes::Secondary::CriticalHitResistance.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthRegenerationAttribute()).AddLambda(Lambda, Attributes::Secondary::HealthRegeneration.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetManaRegenerationAttribute()).AddLambda(Lambda, Attributes::Secondary::ManaRegeneration.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(Lambda, Attributes::Secondary::MaxHealth.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute()).AddLambda(Lambda, Attributes::Secondary::MaxMana.GetTag());
	
}

void UAttributeMenuWidgetController::WidgetControllerParametersChanged(const FWidgetControllerParams& WCParams)
{
	BroadcastInitialValues();
}