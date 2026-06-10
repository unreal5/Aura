// 版权没有，拷贝自由。


#include "UI/AttributeMenu/AttributeMenu.h"

#include "Components/SizeBox.h"

void UAttributeMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateFrameSize();
}

void UAttributeMenu::UpdateFrameSize() const
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}