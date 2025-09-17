// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/Widget/AuraUserWidget.h"
#include "TextValueRow.generated.h"

class UFramedValue;
class UTextBlock;
class USizeBox;
/**
 * 
 */
UCLASS()
class AURA_API UTextValueRow : public UAuraUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FramedProperties")
	FGameplayTag AttributeTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FramedProperties")
	float BoxWidth = 760.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FramedProperties")
	float BoxHeight = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FramedProperties", meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Label;

	UFUNCTION(BlueprintCallable)
	void SetLabelText(const FText& NewText);

	UFUNCTION(BlueprintCallable)
	void SetNumericalValueFloat(float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetNumericalValueInt(int32 NewValue);
protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties", meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UFUNCTION(BlueprintCallable)
	void UpdateFrameSize();

	// WBP_FramedValue
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UFramedValue> WBP_FramedValue;
	// UNamedSlot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties", meta = (BindWidget))
	TObjectPtr<class UNamedSlot> NamedSlot_Base;
};
