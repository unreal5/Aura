// 版本没有，拷贝自由。


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	auto AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(AS);

	/*
	// strength
	auto Info = AttributeInfo->FindAttributeInfoForTag(AuraGameplayTags::Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
	*/
	for (auto& [Key, Value] : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Key, Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	auto AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(AS);
	for (auto& [Key, Value] : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Value()).AddLambda([this, Key, Value](auto)
		{
			BroadcastAttributeInfo(Key, Value());
		});
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	auto Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
