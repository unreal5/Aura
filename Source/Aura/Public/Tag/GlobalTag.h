// 版权没有，拷贝自由。

#pragma once
#include "NativeGameplayTags.h"

namespace GlobalTag
{
	// Primary Attributes
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);
	// Vital Attributes
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital_Health);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital_MaxHealth);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital_Mana);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vital_MaxMana);

	// Message tags
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_HealthPotion);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_ManaPotion);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_HealthCrystal);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_ManaCrystal);
}