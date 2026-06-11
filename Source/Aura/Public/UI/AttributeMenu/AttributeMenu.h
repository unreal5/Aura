// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AttributeMenu.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAttributeMenu : public UAuraUserWidget
{
	GENERATED_BODY()

protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AuraAttributeMenu")
	float BoxWidth = 805.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AuraAttributeMenu")
	float BoxHeight = 906.f;

	// 主属性
	UPROPERTY(meta = (BindWidget))
	class UTextValueRow* Strength;
	UPROPERTY(meta = (BindWidget))
	class UTextValueRow* Intelligence;
	UPROPERTY(meta = (BindWidget))
	class UTextValueRow* Resilience;
	UPROPERTY(meta = (BindWidget))
	class UTextValueRow* Vigor;
	
	// 次属性：和AuraAttributeSet 中一致
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "AuraAttributeMenu | Secondary Attributes")
	TSubclassOf<class UTextValueRow> SecondaryAttributeRowClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AuraAttributeMenu | Secondary Attributes")
	TArray<class UTextValueRow*> SecondaryAttributes;

private:
	void UpdateFrameSize() const;
};