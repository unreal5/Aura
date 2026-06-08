// 版权没有，拷贝自由。


#include "UI/AttributeMenu/FrameValue.h"

#include "Components/SizeBox.h"


void UFrameValue::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateFrameSize();
}

void UFrameValue::UpdateFrameSize()
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}