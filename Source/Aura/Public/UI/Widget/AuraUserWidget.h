// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * UAuraUserWidget 最重要的特征是拥有WidgetController，WidgetController是一个UObject，负责管理Widget的逻辑。
 * WidgetController负责收集数据，处理输入，更新UI等逻辑，而Widget只负责展示UI。
 * 这样设计的好处是将UI逻辑和展示分离（AuraUserWidget单向依赖WidgetController,），WidgetController可以被多个Widget共享，减少代码重复，提高代码复用性。
 * 同时WidgetController也可以被测试，方便进行单元测试和集成测试。
 */
UCLASS(Abstract, Blueprintable)
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Aura | Setter")
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly, Category = "Aura | Controller")
	TObjectPtr<UObject> WidgetController;
protected:
	// 设置WidgetController时会调用这个函数
	UFUNCTION(BlueprintNativeEvent, Category = "Aura | Callback Event")
	void WidgetControllerSet();
};
