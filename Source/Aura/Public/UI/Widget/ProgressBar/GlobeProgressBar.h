// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "GlobeProgressBar.generated.h"

class UTextBlock;
class UImage;
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
	virtual void NativePreConstruct() override;

	// SizeBox
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GlobeProperties")
	float BoxWidth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GlobeProperties")
	float BoxHeight = 100.0f;

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_Root;

	// background image
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GlobeProperties")
	FSlateBrush BackgroundBrush;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Background;


	// progress bar
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GlobeProperties")
	FSlateBrush ProgressBarFillImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GlobeProperties")
	float GlobePadding = 10.0f;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Globe;

	// image glass
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Glass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GlobeProperties")
	FSlateBrush GlassBrush;

	// text block
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ExtraInfo;
public:
	void UpdateBoxSize();

	void UpdateBackgroundBrush();
	
	void UpdateGlobeImage();
	void UpdateGlobePadding();
	
	void UpdateGlassBrush();
	void UpdateGlassPadding();

	UFUNCTION(BlueprintCallable)
	void SetProgressBarPercent(float InValue, float InMaxValue);
};
