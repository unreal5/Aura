// 版权没有，拷贝自由。


#include "UI/AttributeMenu/TextValueRow.h"

#include "Components/SizeBox.h"

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
