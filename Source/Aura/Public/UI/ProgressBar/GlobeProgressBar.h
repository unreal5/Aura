// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "GlobeProgressBar.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;
class USizeBox;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Globe属性")
	float BoxWidth = 256.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Globe属性")
	float BoxHeight = 256.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	// Background
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Image_Background;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Globe属性")
	FSlateBrush BackgroundBrush;

	// ProgressBar
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_Globe;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Globe属性")
	FSlateBrush ProgressBarFillImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Globe属性")
	float GlobePadding = 10.f;

	// Glass
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Globe属性", meta=(BindWidget))
	TObjectPtr<UImage> Image_Glass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Globe属性")
	FSlateBrush GlassBrush;

	// info
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Info;
protected:
	UFUNCTION(BlueprintCallable)
	void SetProgressBarPercent(float InPercent);
private:
	void UpdateSizeBox() const;
	void UpdateBackground() const;
	void SetGlobeImage() const;
	void UpdateGlobePadding() const;
	void UpdateGlassBrush() const;
	void UpdateGlassPadding() const;
};
