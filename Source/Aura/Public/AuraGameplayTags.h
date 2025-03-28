// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace AuraGameplayTags
{
	// primary attributes
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);
	// secondary attributes
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana);

	// input actions
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);

	// event tags for montages
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Eventg_Montage_FireBolt);
	
	
}
