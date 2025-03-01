// 版权没有，拷贝自由。


#include "UI/Widget/ProgressBar/GlobeProgressBar.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"


void UGlobeProgressBar::NativePreConstruct()
{
	// Super::NativePreConstruct()会调用蓝图类PreConstruct函数
	Super::NativePreConstruct();

	UpdateBoxSize();
	UpdateBackgroundBrush();

	UpdateGlobeImage();
	UpdateGlobePadding();

	UpdateGlassBrush();
	UpdateGlassPadding();
}

void UGlobeProgressBar::UpdateBoxSize()
{
	SizeBox_Root->SetWidthOverride(BoxWidth);
	SizeBox_Root->SetHeightOverride(BoxHeight);
}

void UGlobeProgressBar::UpdateBackgroundBrush()
{
	Image_Background->SetBrush(BackgroundBrush);
}

void UGlobeProgressBar::UpdateGlobeImage()
{
	FProgressBarStyle ProgressBarStyle;
	ProgressBarStyle.BackgroundImage.TintColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
	ProgressBarStyle.FillImage = ProgressBarFillImage;

	ProgressBar_Globe->SetWidgetStyle(ProgressBarStyle); 
}

void UGlobeProgressBar::UpdateGlobePadding()
{
	UWidgetLayoutLibrary::SlotAsOverlaySlot(ProgressBar_Globe)->SetPadding(FMargin(GlobePadding));
}


void UGlobeProgressBar::UpdateGlassBrush()
{
	Image_Glass->SetBrush(GlassBrush);
}

void UGlobeProgressBar::UpdateGlassPadding()
{
	UWidgetLayoutLibrary::SlotAsOverlaySlot(Image_Glass)->SetPadding(FMargin(GlobePadding));
}

void UGlobeProgressBar::SetProgressBarPercent(float InValue, float InMaxValue)
{
	float Percent = UKismetMathLibrary::SafeDivide(InValue, InMaxValue);
	ProgressBar_Globe->SetPercent(Percent);
	TextBlock_ExtraInfo->SetText(FText::FromString(FString::Printf(TEXT("%.1f / %.1f"), InValue, InMaxValue)));
}
