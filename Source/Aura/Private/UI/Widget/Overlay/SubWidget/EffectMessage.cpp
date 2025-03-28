// 版权没有，拷贝自由。


#include "UI/Widget/Overlay/SubWidget/EffectMessage.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UEffectMessage::SetImageAndText(UTexture2D* InImage, const FText& InText)
{
	if (Image_Icon)
	{
		FSlateBrush Brush;
		Brush.DrawAs = ESlateBrushDrawType::Image;
		Brush.Tiling = ESlateBrushTileType::NoTile;
		Brush.Mirroring = ESlateBrushMirrorType::NoMirror;
		Brush.ImageSize = ImageSize;
		
		Brush.SetResourceObject(InImage);
		Image_Icon->SetBrush(Brush);

		PlayStartAnimation();
	}

	if (TextBlock_Message)
	{
		TextBlock_Message->SetText(InText);
	}
}
