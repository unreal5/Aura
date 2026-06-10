// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "FrameValue.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class AURA_API UFrameValue : public UAuraUserWidget
{
	GENERATED_BODY()

protected:
	void NativePreConstruct() override;

public:
	// SizeBox_Root是一个SizeBox组件，用于控制框架的大小。通过编辑BoxWidth和BoxHeight属性，可以调整框架的宽度和高度。
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties")
	float BoxWidth = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties")
	float BoxHeight = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties", meta = (BindWidget))
	UImage* Image_Background;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties")
	FSlateBrush BackgroundBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties", meta = (BindWidget))
	UImage* Image_Border;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties", meta = (BindWidget))
	UTextBlock* TextBlock_Value;
private:
	void UpdateFrameSize() const;
	void UpdateBackgroundImage() const;
};