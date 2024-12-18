// 版本没有，拷贝自由。


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& InAttributeTag, bool bLogNotFound) const
{
	for (const auto& Info : AttributeInformation)
	{
		if (InAttributeTag.MatchesTagExact(Info.AttributeTag))
		{
			return Info;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeInfo not found for tag %s"), *InAttributeTag.ToString());
	}
	return FAuraAttributeInfo{};
}
