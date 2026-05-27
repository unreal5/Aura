// 版权没有，拷贝自由。


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!IsValid(OverlayWidgetController))
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		
		// 绑定属性变化的回调
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("请在%s中设置OverlayWidgetClass"), *GetClass()->GetName());
	checkf(OverlayWidgetControllerClass, TEXT("请在%s中设置OverlayWidgetControllerClass"), *GetClass()->GetName());

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	if (!OverlayWidget)
	{
		checkf(false, TEXT("无法创建OverlayWidget，请确保%s设置的OverlayWidgetClass正确"), *GetClass()->GetName());
		return;
	}

	FWidgetControllerParams WCParams{PC, PS, ASC, AS};
	GetOverlayWidgetController(WCParams);
	checkf(IsValid(OverlayWidgetController), TEXT("无法创建OverlayWidgetController，请确保%s设置的OverlayWidgetControllerClass正确"),
	       *GetClass()->GetName());
	
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValues();
	
	OverlayWidget->AddToViewport();
}
