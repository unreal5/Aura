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
public:
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties")
	float BoxWidth = 805.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties")
	float BoxHeight = 906.f;
private:
	void UpdateFrameSize() const;
};