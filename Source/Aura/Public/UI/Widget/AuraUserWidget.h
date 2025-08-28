// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * Widget 有 Controller 的概念，Controller 负责处理逻辑，Widget 负责展示
 * 如果设置 WidgetController，通常我们需要订阅一些消息，因此增加了对应的函数，便于子类按需扩展。
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	// 使用UObject做为控制器，是为了更好的扩展性。
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
