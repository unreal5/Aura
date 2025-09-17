// 版权没有，拷贝自由。


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParameters& InParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		if (OverlayWidgetController)
		{
			OverlayWidgetController->SetWidgetControllerParameters(InParams);
			OverlayWidgetController->BindCallbacksToDependencies();
		}
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParameters& InParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController =
			NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		if (AttributeMenuWidgetController)
		{
			AttributeMenuWidgetController->SetWidgetControllerParameters(InParams);
			AttributeMenuWidgetController->BindCallbacksToDependencies();
		}
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerState,
                           UAbilitySystemComponent* InASC, UAttributeSet* InAttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is nullptr. Please set it in the BP_AuraHUD."));
	checkf(OverlayWidgetControllerClass,
	       TEXT("OverlayWidgetControllerClass is nullptr. Please set it in the BP_AuraHUD."));

	GetOverlayWidgetController(FWidgetControllerParameters{InPlayerController, InPlayerState, InASC, InAttributeSet});

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	if (OverlayWidget)
	{
		OverlayWidget->SetWidgetController(OverlayWidgetController);
		OverlayWidgetController->BroadcastInitialValues();
		OverlayWidget->AddToViewport();
	}
}
