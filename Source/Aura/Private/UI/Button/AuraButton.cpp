// 版权没有，拷贝自由。


#include "UI/Button/AuraButton.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UAuraButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateBoxSize();
	UpdateBorderBrush();
	UpdateButtonStyle();
	UpdateButtonText();
}

void UAuraButton::UpdateBoxSize() const
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}

void UAuraButton::UpdateBorderBrush() const
{
	if (Image_Border)
	{
		Image_Border->SetBrush(BorderBrush);
	}
}

void UAuraButton::UpdateButtonStyle() const
{
	if (Button_Main)
	{
		FButtonStyle ButtonStyle;

		ButtonStyle.SetNormal(ButtonStyle_Normal);
		ButtonStyle.SetHovered(ButtonStyle_Hovered);
		ButtonStyle.SetPressed(ButtonStyle_Pressed);
		ButtonStyle.SetDisabled(ButtonStyle_Disabled);

		Button_Main->SetStyle(ButtonStyle);
	}
}

void UAuraButton::UpdateButtonText() const {
	if (ButtonText)
	{
		// 禁用文字的交互
		ButtonText->SetVisibility(ESlateVisibility::HitTestInvisible);
		ButtonText->SetText(Text);
	}
}
