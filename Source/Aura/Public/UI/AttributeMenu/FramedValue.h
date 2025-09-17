// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "FramedValue.generated.h"

class UImage;
class USizeBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class AURA_API UFramedValue : public UAuraUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FramedProperties")
	float BoxWidth = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FramedProperties")
	float BoxHeight = 45.f;

	void SetValueText(const FText& NewText);
protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties", meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UFUNCTION(BlueprintCallable)
	void UpdateFrameSize();

	// 背景图片
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties")
	FSlateBrush BackgroundBrush;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties", meta = (BindWidget))
	TObjectPtr<UImage> Image_Background;

	UFUNCTION(BlueprintCallable)
	void UpdateBackgroundBrush();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties", meta = (BindWidget))
	TObjectPtr<UImage> Image_Border;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Value;
};
