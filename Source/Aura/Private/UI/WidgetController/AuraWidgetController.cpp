// 版权没有，拷贝自由。


#include "UI/WidgetController/AuraWidgetController.h"

FWidgetControllerParams::FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState,
                                                 UAbilitySystemComponent* InAbilitySystemComponent,
                                                 UAttributeSet* InAttributeSet)
	: PlayerController(InPlayerController)
	  , PlayerState(InPlayerState)
	  , AbilitySystemComponent(InAbilitySystemComponent)
	  , AttributeSet(InAttributeSet)
{
}

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InParams)
{
	PlayerController = InParams.PlayerController;
	PlayerState = InParams.PlayerState;
	AbilitySystemComponent = InParams.AbilitySystemComponent;
	AttributeSet = InParams.AttributeSet;
}
