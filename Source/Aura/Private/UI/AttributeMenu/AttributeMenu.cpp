// 版权没有，拷贝自由。


#include "UI/AttributeMenu/AttributeMenu.h"

#include "Components/SizeBox.h"
#include "UI/AttributeMenu/TextValueRow.h"

void UAttributeMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateFrameSize();
}

void UAttributeMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (IsValid(SecondaryAttributeRowClass))
	{
		
	}
}

void UAttributeMenu::UpdateFrameSize() const
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}