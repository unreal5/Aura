// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "UI/Widget/AuraUserWidget.h"
#include "TextValueRow.generated.h"

class UTextBlock;
class UFrameValue;
/**
 * 
 */
UCLASS()
class AURA_API UTextValueRow : public UAuraUserWidget
{
	GENERATED_BODY()
protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;
public:
	// SizeBox_Root是一个SizeBox组件，用于控制框架的大小。通过编辑BoxWidth和BoxHeight属性，可以调整框架的宽度和高度。
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "文本行属性")
	float BoxWidth = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "文本行属性")
	float BoxHeight = 60.f;
	
	UPROPERTY(meta = (BindWidget),EditAnywhere, BlueprintReadWrite, Category = "文本行属性")
	UTextBlock* TextBlock_AttributeName;
	UPROPERTY(meta = (BindWidget),EditAnywhere, BlueprintReadWrite, Category = "文本行属性")
	UFrameValue* WBP_FrameValue;
	UPROPERTY(meta = (BindWidget),EditAnywhere, BlueprintReadWrite, Category = "文本行属性")
	UNamedSlot* NamedSlot_Content;
	
	UFUNCTION(BlueprintCallable, Category = "文本行功能")
	void SetLableText(const FText& InName);
	
	UFUNCTION(BlueprintCallable, Category = "文本行功能")
	void SetNumericalValue(float NewValue);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "标签")
	FGameplayTag AttributeTag;
private:
	void UpdateFrameSize() const;
	
	UFUNCTION()
	void UpdateAttributeInfo(const FAuraAttributeInfo& AttributeInfo);
};