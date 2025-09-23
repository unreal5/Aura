// 版权没有，拷贝自由。


#include "Tag/GlobalTag.h"


namespace GlobalTag
{
	//define primary attributes
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Resilience, "Attributes.Primary.Resilience");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Strength, "Attributes.Primary.Strength");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Vigor, "Attributes.Primary.Vigor");
	// define vital attributes
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vital_Health, "Attributes.Vital.Health");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vital_Mana, "Attributes.Vital.Mana");
	// define secondary attributes
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_Armor, "Attributes.Secondary.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana");
	// define message tags
	UE_DEFINE_GAMEPLAY_TAG(Message_HealthPotion, "Message.HealthPotion");
	UE_DEFINE_GAMEPLAY_TAG(Message_ManaPotion, "Message.ManaPotion");
	UE_DEFINE_GAMEPLAY_TAG(Message_HealthCrystal, "Message.HealthCrystal");
	UE_DEFINE_GAMEPLAY_TAG(Message_ManaCrystal, "Message.ManaCrystal");
	// define input tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LMB, "Input.LMB");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_RMB, "Input.RMB");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MMB, "Input.MMB");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_1, "Input.1");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_2, "Input.2");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_3, "Input.3");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_4, "Input.4");
	// define event tags
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_FireBolt, "Event.Montage.FireBolt");
}
