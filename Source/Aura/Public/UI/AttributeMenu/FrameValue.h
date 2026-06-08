// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "FrameValue.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UFrameValue : public UAuraUserWidget
{
	GENERATED_BODY()

protected:
	void NativePreConstruct() override;

public:
	// SizeBox_Root是一个SizeBox组件，用于控制框架的大小。通过编辑BoxWidth和BoxHeight属性，可以调整框架的宽度和高度。
	UPROPERTY(meta = (BindWidget))
	class USizeBox* SizeBox_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties")
	float BoxWidth = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrameProperties")
	float BoxHeight = 45.f;

private:
	void UpdateFrameSize();
};