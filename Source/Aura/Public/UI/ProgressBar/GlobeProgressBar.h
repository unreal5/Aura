// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/AuraUserWidget.h"
#include "GlobeProgressBar.generated.h"

class UTextBlock;
class UImage;
class UOverlay;
class USizeBox;
class UProgressBar;
/**
 * 
 */
UCLASS()
class AURA_API UGlobeProgressBar : public UAuraUserWidget
{
	GENERATED_BODY()

protected:
	void NativePreConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="Widget宽度"))
	float BoxWidth = 128.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="Widget高度"))
	float BoxHeight = 128.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="Widget背景图片"))
	FSlateBrush BackgroundBrush;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="Globe Padding"))
	float GlobePadding = 6.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="进度条图片"))
	FSlateBrush ProgressBarFillImage;
	
	// Glass
	UPROPERTY(BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="Glass Padding"))
	float GlassPadding = 6.f;
	UPROPERTY(BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="进度条图片"))
	FSlateBrush GlassBrush;
	
	UFUNCTION(BlueprintCallable, Category = "Aura | GlobeProgressBar")
	void SetProgressBarPercent(float Percent) const;
	
	UFUNCTION(BlueprintCallable, Category = "Aura | GlobeProgressBar")
	void SetProgressBarText(const FText& InText) const;
	
	UFUNCTION(BlueprintCallable, Category = "Aura | GlobeProgressBar")
	void SetNumerator(float InNumerator);
	
	UFUNCTION(BlueprintCallable, Category = "Aura | GlobeProgressBar")
	void SetDenominator(float InDenominator);
	
	// 利用分子分母设置进度条百分比所需标签
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="分子标签"))
	FGameplayTag NumeratorTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Globe属性", meta=(ToolTip="分母标签"))
	FGameplayTag DenominatorTag;

	// 实现基类回调
	void WidgetControllerSet_Implementation() override;
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Overlay_Root;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Background;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Globe;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Glass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Percent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Globe属性")
	float Numerator = 1.f;
	UPROPERTY(BlueprintReadOnly, Category = "Globe属性")
	float Denominator = 1.f;
private:
	void UpdateBoxSize() const;
	void UpdateOverlay() const;
	void UpdateBackgroundBrush() const;
	void UpdateGlobeImage() const;
	void UpdateGlassImage() const;
	
	// 附加功能
	void UpdateProgressBarAndText() const;
	UFUNCTION()
	void OnAttributeChangedWithTag(FGameplayTag AttributeTag, float NewValue);
};
