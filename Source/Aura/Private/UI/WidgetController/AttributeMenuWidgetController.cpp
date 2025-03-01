// 版权没有，拷贝自由。


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	auto AS = Cast<UAuraAttributeSet>(AttributeSet.Get());
	if (!AS) return;
	check(AttributeInfo);


	//begin testing
	// auto ASC = AS->GetOwningAbilitySystemComponentChecked();
	// auto StrengthInfo = AttributeInfo->FindAttributeInfoForTag(AuraGameplayTags::Attributes_Primary_Strength);
	// StrengthInfo.AttributeValue = ASC->GetNumericAttribute(UAuraAttributeSet::GetStrengthAttribute());
	// AttributeInfoDelegate.Broadcast(StrengthInfo);
	// return;
	//end testing
	//Strength

	/*
	// 测试
	UClass* Class = AS->GetClass();
	for (TFieldIterator<FProperty> PropIt(Class); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;
		FGameplayAttribute Attribute(Property);
		if (Attribute.IsValid())
		{
			FString AttributeName = Property->GetName();
			UE_LOG(LogTemp, Log, TEXT("Found Attribute: %s"), *AttributeName);
		}
	}
	// 结束测试
	*/
	for (auto& [Tag, Functor] : AS->TagToAttributeMap)
	{
		BroadcastAttributeInfo(Tag,Functor());
		/*
		auto Info = AttributeInfo->FindAttributeInfoForTag(Tag);
		
		Info.AttributeValue = Functor().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
		*/
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	if (!AbilitySystemComponent) return;
	auto AS = CastChecked<UAuraAttributeSet>(AttributeSet.Get());
	for (auto[Tag, Functor] : AS->TagToAttributeMap)
    {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Functor()).AddLambda([Functor, Tag, this](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Tag,Functor());
			/*
			auto Info = AttributeInfo->FindAttributeInfoForTag(Tag);
			Info.AttributeValue = Data.NewValue;
			AttributeInfoDelegate.Broadcast(Info);
			*/
		});
		
    }
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	auto Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
