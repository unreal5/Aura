// 版权没有，拷贝自由。


#include "HUD/AuraHUD.h"
#include "UI/Widget/Overlay/AuraWidgetOverlay.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (nullptr == OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);

		if (nullptr == OverlayWidgetController)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create overlay widget controller"));
		}
	}

	if (OverlayWidgetController)
	{
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (nullptr == AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);

		if (AttributeMenuWidgetController == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create AttributeMenuWidgetController"));
		}
	}

	if (AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController->SetWidgetControllerParams(Params);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 先创建控制器
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is null"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is null"));

	// 创建 OverlayWidget
	OverlayWidget = CreateWidget<UAuraWidgetOverlay>(GetWorld(), OverlayWidgetClass);
	if (!OverlayWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create overlay widget"));
		return;
	}

	FWidgetControllerParams Params{PC, PS, ASC, AS};
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(Params);
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();


	OverlayWidget->AddToViewport();
}
