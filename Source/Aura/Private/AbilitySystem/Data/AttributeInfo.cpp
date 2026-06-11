// 版权没有，拷贝自由。


#include "AbilitySystem/Data/AttributeInfo.h"
FAuraAttributeInfo UAttributeInfo::GetAttributeInfoByTag(const FGameplayTag& InTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag == InTag)
		{
			return Info;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attribute with tag %s not found in AttributeInfo."), *InTag.ToString());
	}
	
	return FAuraAttributeInfo();
}