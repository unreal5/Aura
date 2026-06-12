// 版权没有，拷贝自由。


#include "UI/AttributeMenu/TextValueRow.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "UI/AttributeMenu/FrameValue.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

void UTextValueRow::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateFrameSize();
}

void UTextValueRow::NativeConstruct()
{
	Super::NativeConstruct();

	UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(this)->AttributeInfoDelegate.AddDynamic(this, &ThisClass::UpdateAttributeInfo);
}

void UTextValueRow::SetLableText(const FText& InName)
{
	if (TextBlock_AttributeName)
	{
		TextBlock_AttributeName->SetText(InName);
	}
}

void UTextValueRow::SetNumericalValue(float NewValue)
{
	if (WBP_FrameValue)
	{
		FNumberFormattingOptions NumberFormatOptions;
		NumberFormatOptions.MaximumFractionalDigits = 1; // 设置小数部分的最大位数为1
		FText ValueText = FText::AsNumber(NewValue, &NumberFormatOptions);
		WBP_FrameValue->SetValueText(ValueText);
	}
}

void UTextValueRow::UpdateFrameSize() const
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}

void UTextValueRow::UpdateAttributeInfo(const FAuraAttributeInfo& AttributeInfo)
{
	if (AttributeInfo.AttributeTag != AttributeTag) return;
	SetLableText(AttributeInfo.AttributeName);
	SetNumericalValue(AttributeInfo.AttributeValue);
}