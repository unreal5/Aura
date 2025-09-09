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
	virtual void NativePreConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UImage* Image_Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* Text_Message;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D ImageSize = FVector2D(64, 64);

	UFUNCTION(BlueprintCallable)
	void SetImageAndText(UTexture2D* InIcon, const FText& InText);
	
	UFUNCTION(BlueprintCallable)
	void SetMessageText(const FText& InText);
	UFUNCTION(BlueprintCallable)
	void SetMessageIcon(UTexture2D* InIcon);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAllAnimation();
};
