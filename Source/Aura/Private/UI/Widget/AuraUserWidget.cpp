// 版本没有，拷贝自由。


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InNewWidgetController)
{
	WidgetController = InNewWidgetController;
	WidgetControllerSet();
}
