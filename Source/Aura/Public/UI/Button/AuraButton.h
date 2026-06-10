// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraButton.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class AURA_API UAuraButton : public UAuraUserWidget
{
	GENERATED_BODY()

protected:
	void NativePreConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	float BoxWidth = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	float BoxHeight = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	FSlateBrush ImageBorderBrush;
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Border;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性", meta=(BindWidget))
	UButton* ClickableButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	FSlateBrush ImageBorderBrush_Pressed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	FSlateBrush ImageBorderBrush_Hovered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	FSlateBrush ImageBorderBrush_Disabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	FSlateBrush ImageBorderBrush_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性", meta=(BindWidget))
	UTextBlock* TextBlock_ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	FString ButtonText = TEXT("X");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "按钮属性")
	FSlateFontInfo ButtonTextFont;

private:
	void UpdateSizeBox() const;
	void UpdateBorderImage() const;
	void UpdateButtonStyle() const;
	void UpdateTextBlock() const;
};
