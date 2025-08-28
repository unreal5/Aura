// 版权没有，拷贝自由。


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	// 有时我们可能需要强制重新触发事件，因此不做相等性判断。
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
