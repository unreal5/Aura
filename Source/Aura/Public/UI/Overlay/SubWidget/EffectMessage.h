// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "EffectMessage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UEffectMessage : public UAuraUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UTextBlock> MessageText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UImage> MessageImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D ImageSize = FVector2D(75.f, 75.f);
	
	UFUNCTION(BlueprintCallable, Category = "Aura | EffectMessage")
	void SetImageAndText(const FText& InText, UTexture2D* InTexture);
	UFUNCTION(BlueprintImplementableEvent, Category = "Aura | EffectMessage")
	void PlayAnimationAndDestroy();
};
