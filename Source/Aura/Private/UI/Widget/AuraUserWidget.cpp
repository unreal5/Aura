// 版权没有，拷贝自由。


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	if (InWidgetController == WidgetController) return;

	WidgetController = InWidgetController;
	WidgetControllerSet();
}
