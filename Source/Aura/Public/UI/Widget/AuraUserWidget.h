// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 使用MVC模式的Widget基类，用于处理UI事件
 * 由此类派生的Widget类，都有控制器的概念，如果控制器播报事件，Widget会接收到事件。
 * 由此，AuraUserWidget类依赖于控制器类；但是控制器类不依赖于Widget类，它甚至并不知道Widget类。
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 设置控制器
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InNewWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// 无论何时，只要设置控制器(调用SetWidgetController()函数)，就会调用此函数
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
