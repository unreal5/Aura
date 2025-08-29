// 版权没有，拷贝自由。


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParameters(const FWidgetControllerParameters& InParameters)
{
	PlayerController = InParameters.PlayerController;
	PlayerState = InParameters.PlayerState;
	AbilitySystemComponent = InParameters.AbilitySystemComponent;
	AttributeSet = InParameters.AttributeSet;
}
