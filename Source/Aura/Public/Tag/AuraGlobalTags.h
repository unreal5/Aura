// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

// namespace AuraAttributeTags
// {
// 	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Health);
// 	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxHealth);
// 	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mana);
// 	UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxMana);
// }

namespace Attributes
{
	namespace Primary
	{
		
	}

	namespace Vital
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Health);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxHealth);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mana);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxMana);
	}
}

namespace Message
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthPotion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthCrystal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ManaPotion);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ManaCrystal);
}
