// 版权没有，拷贝自由。


#include "UI/ProgressBar/GlobeProgressBar.h"

#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "UI/WidgetController/OverlayWidgetController.h"


void UGlobeProgressBar::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateBoxSize();
	UpdateOverlay();
	// 设置Image_Background
	UpdateBackgroundBrush();
	// 设置ProgressBar	
	UpdateGlobeImage();
	// 设置Glass
	UpdateGlassImage();
}

void UGlobeProgressBar::SetProgressBarPercent(float Percent) const
{
	if (ProgressBar_Globe)
	{
		ProgressBar_Globe->SetPercent(Percent);
	}
}

void UGlobeProgressBar::SetProgressBarText(const FText& InText) const
{
	if (Text_Percent)
	{
		Text_Percent->SetText(InText);
	}
}

void UGlobeProgressBar::SetNumerator(float InNumerator)
{
	Numerator = InNumerator;
	UpdateProgressBarAndText();
}

void UGlobeProgressBar::SetDenominator(float InDenominator)
{
	Denominator = InDenominator;
	UpdateProgressBarAndText();
}

void UGlobeProgressBar::WidgetControllerSet_Implementation()
{
	Super::WidgetControllerSet_Implementation();
	
	UOverlayWidgetController* OverlayController = Cast<UOverlayWidgetController>(WidgetController);
	if (!OverlayController) return;
	
	OverlayController->OnAttributeChangedWithTag.AddDynamic(this, &UGlobeProgressBar::OnAttributeChangedWithTag);
}

void UGlobeProgressBar::UpdateBoxSize() const
{
	if (SizeBox_Root)
	{
		SizeBox_Root->SetWidthOverride(BoxWidth);
		SizeBox_Root->SetHeightOverride(BoxHeight);
	}
}

void UGlobeProgressBar::UpdateOverlay() const
{
	/*
	* 在 UMG 里，编辑器中看到的 Horizontal Alignment 一般不是 UImage 自己的属性，而是：这个控件在父容器里的 Slot 属性
	* 所以在 C++ 里，通常不是写：Image_Background->SetHorizontalAlignment(...); // 通常不对
	* 而是要先拿到它的 Slot，再根据 Slot 类型设置。
	*/
	if (auto OverlaySlot = Cast<UOverlaySlot>(Image_Background->Slot))
	{
		OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
		OverlaySlot->SetVerticalAlignment(VAlign_Fill);
	}
}

void UGlobeProgressBar::UpdateBackgroundBrush() const
{
	if (Image_Background)
	{
		Image_Background->SetBrush(BackgroundBrush);
	}
}

void UGlobeProgressBar::UpdateGlobeImage() const
{
	if (!ProgressBar_Globe) return;

	auto OverlaySlot = Cast<UOverlaySlot>(ProgressBar_Globe->Slot);
	if (!OverlaySlot) return;

	// 设置 ProgressBar 的对齐方式为 Fill
	OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
	OverlaySlot->SetVerticalAlignment(VAlign_Fill);
	// 设置 ProgressBar 的Padding
	OverlaySlot->SetPadding(FMargin{GlobePadding});

	// ProgressBar_Globe 需要设置样式，才能正确显示。
	// 这项工作很繁琐，所以我们在编辑器里设置好一个样式，命名为 "GlobeProgressBarStyle"，然后在这里直接引用它。
	FProgressBarStyle BarStyle = ProgressBar_Globe->GetWidgetStyle();
	BarStyle.FillImage = ProgressBarFillImage;
	BarStyle.FillImage.DrawAs = ESlateBrushDrawType::Image;
	BarStyle.BackgroundImage.TintColor = FSlateColor(FLinearColor::Transparent);
	ProgressBar_Globe->SetWidgetStyle(BarStyle);

	// 设置填充颜色为白色，这样才能正确显示 FillImage 的颜色。
	ProgressBar_Globe->SetFillColorAndOpacity(FLinearColor::White);

	ProgressBar_Globe->SetBarFillType(EProgressBarFillType::BottomToTop);
}

void UGlobeProgressBar::UpdateGlassImage() const
{
	if (!IsValid(Image_Glass)) return;
	auto OverlaySlot = Cast<UOverlaySlot>(Image_Glass->Slot);
	if (!OverlaySlot) return;
	// 设置 Image_Glass 的对齐方式为 Fill
	OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
	OverlaySlot->SetVerticalAlignment(VAlign_Fill);
	OverlaySlot->SetPadding(FMargin{GlassPadding});

	Image_Glass->SetBrush(GlassBrush);
}

void UGlobeProgressBar::UpdateProgressBarAndText() const
{
	SetProgressBarPercent(FMath::SafeDivide(Numerator, Denominator));
	FString Text = FString::Printf(TEXT("%.1f / %.1f"), Numerator, Denominator);
	SetProgressBarText(FText::FromString(Text));
}

void UGlobeProgressBar::OnAttributeChangedWithTag(FGameplayTag AttributeTag, float NewValue)
{	if (AttributeTag == NumeratorTag)
	{
		SetNumerator(NewValue);
	}
	else if (AttributeTag == DenominatorTag)
	{
		SetDenominator(NewValue);
	}
}
