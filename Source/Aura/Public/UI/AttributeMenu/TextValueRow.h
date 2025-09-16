// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "TextValueRow.generated.h"

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
	float BoxWidth = 760.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FramedProperties")
	float BoxHeight = 60.f;

protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties", meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UFUNCTION(BlueprintCallable)
	void UpdateFrameSize();

	// UNamedSlot
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FramedProperties", meta = (BindWidget))
	TObjectPtr<class UNamedSlot> NamedSlot_Base;
};
