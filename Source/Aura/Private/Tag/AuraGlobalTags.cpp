// 版权没有，拷贝自由。


#include "Tag/AuraGlobalTags.h"


// UE_DEFINE_GAMEPLAY_TAG(AuraAttributeTags::Health, "AuraAttribute.Health");
// UE_DEFINE_GAMEPLAY_TAG(AuraAttributeTags::MaxHealth, "AuraAttribute.MaxHealth");
// UE_DEFINE_GAMEPLAY_TAG(AuraAttributeTags::Mana, "AuraAttribute.Mana");
// UE_DEFINE_GAMEPLAY_TAG(AuraAttributeTags::MaxMana, "AuraAttribute.MaxMana");


// Attributes::Primary
// Attributes::Vital
UE_DEFINE_GAMEPLAY_TAG(Attributes::Vital::Health, "Attributes.Vital.Health");
UE_DEFINE_GAMEPLAY_TAG(Attributes::Vital::MaxHealth, "Attributes.Vital.MaxHealth");
UE_DEFINE_GAMEPLAY_TAG(Attributes::Vital::Mana, "Attributes.Vital.Mana");
UE_DEFINE_GAMEPLAY_TAG(Attributes::Vital::MaxMana, "Attributes.Vital.MaxMana");


// Message
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message::Tag, "Message", "标识Message标签的根标签");
UE_DEFINE_GAMEPLAY_TAG(Message::HealthPotion, "Message.HealthPotion");
UE_DEFINE_GAMEPLAY_TAG(Message::HealthCrystal, "Message.HealthCrystal");
UE_DEFINE_GAMEPLAY_TAG(Message::ManaPotion, "Message.ManaPotion");
UE_DEFINE_GAMEPLAY_TAG(Message::ManaCrystal, "Message.ManaCrystal");