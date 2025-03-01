// 版权没有，拷贝自由。


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& InAttributeTag, bool bLogNotFound) const
{
	auto FoundItem = AttributeInformation.FindByPredicate([=](const FAuraAttributeInfo& AttributeInfo)
	{
		return AttributeInfo.AttributeTag.MatchesTagExact(InAttributeTag);
	});
	if (!FoundItem && bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeInfo for tag %s not found in %s"), *InAttributeTag.ToString(),
		       *GetName());
		return FAuraAttributeInfo{};
	}
	return *FoundItem;
}
