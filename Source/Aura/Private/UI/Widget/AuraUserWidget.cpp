// 版权没有，拷贝自由。


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UAuraWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UAuraUserWidget::WidgetControllerSet_Implementation()
{
	// do nothing
}
