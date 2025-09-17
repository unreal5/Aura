// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraButton.generated.h"

class UTextBlock;
class UImage;
class USizeBox;
class UButton;

UCLASS()
class AURA_API UAuraButton : public UAuraUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton", meta=(BindWidget))
	TObjectPtr<UImage> Image_Border;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton")
	FSlateBrush BorderBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AuraButton")
	float BoxWidth = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AuraButton")
	float BoxHeight = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton", meta=(BindWidget))
	TObjectPtr<UButton> Button_Main;
	// button style
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton|Style")
	FSlateBrush ButtonStyle_Normal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton|Style")
	FSlateBrush ButtonStyle_Hovered;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton|Style")
	FSlateBrush ButtonStyle_Pressed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton|Style")
	FSlateBrush ButtonStyle_Disabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton", meta=(BindWidget))
	TObjectPtr<UTextBlock> ButtonText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AuraButton")
	FText Text = FText::FromString("+");
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AuraButton", meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	virtual void NativePreConstruct() override;

private:
	void UpdateBoxSize() const;
	void UpdateBorderBrush() const;
	void UpdateButtonStyle() const;
	void UpdateButtonText() const;
};
