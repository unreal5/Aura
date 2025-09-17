// 版权没有，拷贝自由。


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& InTag, bool bLogNotFound) const
{
	for (const auto& Info : AttributeInformations)
	{
		if (Info.AttributeTag.MatchesTagExact(InTag))
			return Info;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttributeInfo not found for tag: %s"), *InTag.ToString());
	}

	return FAuraAttributeInfo{};
}
