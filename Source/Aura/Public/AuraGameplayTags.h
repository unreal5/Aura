// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace AuraGameplayTags
{
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor)

	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana)

	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Fire)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Lightning)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Arcane)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Physical)
	// input tag
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4)

	// set by caller
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical)

	// Ability tag
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Summon)

	// CombatSocket tag
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Weapon)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_RightHand)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_LeftHand)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Tail)

	// Montage attack tag
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MontageTag_Attack_1)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MontageTag_Attack_2)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MontageTag_Attack_3)
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MontageTag_Attack_4)
	
	extern TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;
	// hit react
	AURA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effects_HitReact)
}