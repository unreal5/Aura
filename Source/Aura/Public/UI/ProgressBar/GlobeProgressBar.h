// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "GlobeProgressBar.generated.h"

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

private:
	void UpdateBoxSize() const;
	void UpdateOverlay() const;
	void UpdateBackgroundBrush() const;
	void UpdateGlobeImage() const;
	void UpdateGlassImage() const;
};
