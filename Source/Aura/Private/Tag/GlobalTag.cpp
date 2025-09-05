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
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vital_MaxHealth, "Attributes.Vital.MaxHealth");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vital_Mana, "Attributes.Vital.Mana");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vital_MaxMana, "Attributes.Vital.MaxMana");
	// define message tags
	UE_DEFINE_GAMEPLAY_TAG(Message_HealthPotion, "Message.HealthPotion");
	UE_DEFINE_GAMEPLAY_TAG(Message_ManaPotion, "Message.ManaPotion");
	UE_DEFINE_GAMEPLAY_TAG(Message_HealthCrystal, "Message.HealthCrystal");
	UE_DEFINE_GAMEPLAY_TAG(Message_ManaCrystal, "Message.ManaCrystal");
}
