// 版权没有，拷贝自由。


#include "UI/ProgressBar/GlobeProgressBar.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"

void UGlobeProgressBar::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateSizeBox();
	UpdateBackground();
	SetGlobeImage();
	UpdateGlobePadding();
	UpdateGlassBrush();
	UpdateGlassPadding();
}

void UGlobeProgressBar::SetProgressBarPercent(float InPercent)
{
	if (ProgressBar_Globe)
	{
		ProgressBar_Globe->SetPercent(InPercent);
	}
	if (Text_Info)
	{
		Text_Info->SetText(FText::FromString(FString::Printf(TEXT("%.1f%%"), InPercent * 100.f)));
	}
}

void UGlobeProgressBar::UpdateProgressBarPercent()
{
	float NewPercent = UKismetMathLibrary::SafeDivide(CurrentValue, MaxValue);
	SetProgressBarPercent(NewPercent);
	if (Text_CurrentValue)
	{
		Text_CurrentValue->SetText(FText::FromString(FString::Printf(TEXT("当前：%.1f"), CurrentValue)));
	}
	if (Text_MaxValue)
	{
		Text_MaxValue->SetText(FText::FromString(FString::Printf(TEXT("最大：%.1f"), MaxValue)));
	}
}

void UGlobeProgressBar::UpdateSizeBox() const
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}

void UGlobeProgressBar::UpdateBackground() const
{
	if (Image_Background)
	{
		Image_Background->SetBrush(BackgroundBrush);
	}
}

void UGlobeProgressBar::SetGlobeImage() const
{
	FProgressBarStyle NewStyle;
	NewStyle.BackgroundImage.TintColor = FSlateColor(FColor{1, 1, 1, 0}); //透明
	NewStyle.FillImage = ProgressBarFillImage;
	if (ProgressBar_Globe)
	{
		ProgressBar_Globe->SetWidgetStyle(NewStyle);
	}
}

void UGlobeProgressBar::UpdateGlobePadding() const
{
	FMargin NewPadding{GlobePadding};
	if (ProgressBar_Globe)
	{
		if (auto ProgressBar_Slot = UWidgetLayoutLibrary::SlotAsOverlaySlot(ProgressBar_Globe))
		{
			ProgressBar_Slot->SetPadding(NewPadding);
		}
	}
}

void UGlobeProgressBar::UpdateGlassBrush() const
{
	if (Image_Glass)
	{
		Image_Glass->SetBrush(GlassBrush);
	}
}

void UGlobeProgressBar::UpdateGlassPadding() const
{
	FMargin NewPadding{GlobePadding};
	if (Image_Glass)
	{
		if (auto Glass_Slot = UWidgetLayoutLibrary::SlotAsOverlaySlot(Image_Glass))
		{
			Glass_Slot->SetPadding(NewPadding);
		}
		Image_Glass->SetColorAndOpacity(FLinearColor{1, 1, 1, 0.5f});
	}
}
