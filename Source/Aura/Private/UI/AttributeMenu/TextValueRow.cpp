// 版权没有，拷贝自由。


#include "UI/AttributeMenu/TextValueRow.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "UI/AttributeMenu/FramedValue.h"

void UTextValueRow::SetLabelText(const FText& NewText)
{
	if (TextBlock_Label)
	{
		TextBlock_Label->SetText(NewText);
	}
}

void UTextValueRow::SetNumericalValueFloat(float NewValue)
{
	if (WBP_FramedValue)
	{
		FString ValueString = FString::Printf(TEXT("%.1f"), NewValue);
		FText ValueText = FText::FromString(ValueString);
		WBP_FramedValue->SetValueText(ValueText);
	}
}

void UTextValueRow::SetNumericalValueInt(int32 NewValue)
{
	if (WBP_FramedValue)
	{
		FString ValueString = FString::Printf(TEXT("%d"), NewValue);
		FText ValueText = FText::FromString(ValueString);
		WBP_FramedValue->SetValueText(ValueText);
	}
}

void UTextValueRow::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateFrameSize();
}

void UTextValueRow::UpdateFrameSize()
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}
