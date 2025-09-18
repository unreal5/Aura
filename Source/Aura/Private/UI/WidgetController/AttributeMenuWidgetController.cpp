// 版权没有，拷贝自由。


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Tag/GlobalTag.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	if (!AS) return;
	for (auto& [Tag, GetAttributeFunc] : AS->TagsToAttributesMap)
	{
		// Tag 是临时变时，lambda表达式不可以以引用捕获
		// 因为这个lambda表达式会在这个函数返回后才被调用
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeFunc()).AddLambda(
			[Tag,&GetAttributeFunc,this](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag, GetAttributeFunc());
			});
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	if (!AS) return;

	check(AttributeInfo);

	for (auto& [Tag, GetAttributeFunc] : AS->TagsToAttributesMap)
	{
		BroadcastAttributeInfo(Tag, GetAttributeFunc());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& InTag,
                                                            const FGameplayAttribute& InAttribute) const
{
	auto AttrInfo = AttributeInfo->FindAttributeInfoByTag(InTag);
	AttrInfo.AttributeValue = InAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(AttrInfo);
}
