// 版权没有，拷贝自由。


#include "UI/AttributeMenu/FramedValue.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UFramedValue::SetValueText(const FText& NewText)
{
	if (TextBlock_Value)
	{
		TextBlock_Value->SetText(NewText);
	}
}

void UFramedValue::NativePreConstruct()
{
	Super::NativeConstruct();
	UpdateFrameSize();
	UpdateBackgroundBrush();
}

void UFramedValue::UpdateFrameSize()
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}

void UFramedValue::UpdateBackgroundBrush()
{
	if (Image_Background)
	{
		Image_Background->SetBrush(BackgroundBrush);
	}
}
