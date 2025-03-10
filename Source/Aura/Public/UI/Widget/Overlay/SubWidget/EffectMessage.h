// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "EffectMessage.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class AURA_API UEffectMessage : public UAuraUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Icon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_Message;

	UPROPERTY(EditAnywhere)
	FVector2D ImageSize = FVector2D(75.0f, 75.0f);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayStartAnimation();
	
	UFUNCTION(BlueprintCallable)
	void SetImageAndText(UTexture2D* InImage, const FText& InText);
};
