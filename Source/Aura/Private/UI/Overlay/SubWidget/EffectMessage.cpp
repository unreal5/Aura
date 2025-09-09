// 版权没有，拷贝自由。


#include "UI/Overlay/SubWidget/EffectMessage.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UEffectMessage::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UEffectMessage::SetImageAndText(UTexture2D* InIcon, const FText& InText)
{
	SetMessageText(InText);
	SetMessageIcon(InIcon);
	PlayAllAnimation();
}

void UEffectMessage::SetMessageText(const FText& InText)
{
	if (Text_Message)
	{
		Text_Message->SetText(InText);
	}
}

void UEffectMessage::SetMessageIcon(UTexture2D* InIcon)
{
	if (Image_Icon)
	{
		Image_Icon->SetBrushFromTexture(InIcon);
		FSlateBrush Brush;
		Brush.DrawAs = ESlateBrushDrawType::Image;
		Brush.Tiling = ESlateBrushTileType::NoTile;
		Brush.ImageSize = ImageSize;
		Brush.SetResourceObject(InIcon);
		Image_Icon->SetBrush(Brush);
	}
}
