// 版权没有，拷贝自由。


#include "UI/Button/AuraButton.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UAuraButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateSizeBox();
	UpdateBorderImage();
	UpdateButtonStyle();
	UpdateTextBlock();
}

void UAuraButton::UpdateSizeBox() const
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}

void UAuraButton::UpdateBorderImage() const
{
	if (Image_Border)
	{
		Image_Border->SetBrush(ImageBorderBrush);
	}
}

void UAuraButton::UpdateButtonStyle() const
{
	if (ClickableButton)
	{
		FButtonStyle ButtonStyle;
		ButtonStyle.SetNormal(ImageBorderBrush_Normal);
		ButtonStyle.SetHovered(ImageBorderBrush_Hovered);
		ButtonStyle.SetPressed(ImageBorderBrush_Pressed);
		ButtonStyle.SetDisabled(ImageBorderBrush_Disabled);
		ClickableButton->SetStyle(ButtonStyle);
	}
}

void UAuraButton::UpdateTextBlock() const
{
	if (TextBlock_ButtonText)
	{
		TextBlock_ButtonText->SetText(FText::FromString(ButtonText));
		TextBlock_ButtonText->SetFont(ButtonTextFont);
	}
}
