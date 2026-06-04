// 版权没有，拷贝自由。


#include "UI/Overlay/SubWidget/EffectMessage.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UEffectMessage::SetImageAndText(const FText& InText, UTexture2D* InTexture)
{
	if (MessageText)
	{
		MessageText->SetText(InText);
	}
	
	if (MessageImage && InTexture)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(InTexture);
		Brush.SetImageSize(ImageSize);
		MessageImage->SetBrush(Brush);
	}
	PlayAnimationAndDestroy();
}
