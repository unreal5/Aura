// 版权没有，拷贝自由。


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Tag/GlobalTag.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	if (!AS) return;

	check(AttributeInfo);

	for (auto& [Tag, GetAttributeFunc] : AS->TagsToAttributesMap)
	{
		auto AttrInfo = AttributeInfo->FindAttributeInfoByTag(Tag);
		AttrInfo.AttributeValue = GetAttributeFunc().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(AttrInfo);
	}
}
