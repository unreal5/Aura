// 版权没有，拷贝自由。


#include "UI/Widget/AuraUserWidget.h"

#include "UI/WidgetController/AuraWidgetController.h"


void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	if (InWidgetController == WidgetController) return;

	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UAuraUserWidget::WidgetControllerSet_Implementation()
{
	auto AuraWidgetController = Cast<UAuraWidgetController>(WidgetController);
	if (!AuraWidgetController) return;
	AuraWidgetController->BindCallbacksToDependencies();
	AuraWidgetController->BroadcastInitialValues();
}