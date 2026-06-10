// 版权没有，拷贝自由。


#include "UI/AttributeMenu/FrameValue.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"


void UFrameValue::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateFrameSize();
	UpdateBackgroundImage();
}

void UFrameValue::UpdateFrameSize() const
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}

void UFrameValue::UpdateBackgroundImage() const
{
	if (Image_Background)
	{
		Image_Background->SetBrush(BackgroundBrush);
	}
}