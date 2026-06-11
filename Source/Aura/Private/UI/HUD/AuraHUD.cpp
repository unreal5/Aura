// 版权没有，拷贝自由。


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "UI/Widget/AuraAttributeDebugWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

AAuraHUD::AAuraHUD()
{
	// 设置默认的Debug Widget类
	AttributeDebugWidgetClass = UAuraAttributeDebugWidget::StaticClass();
}

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
	// 初始化调试面板
	InitAttributeDebugPanel(ASC, AS);
}

void AAuraHUD::ToggleAttributeDebugPanel()
{
	if (IsValid(AttributeDebugWidget))
	{
		AttributeDebugWidget->TogglePanelVisibility();
	}
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!IsValid(AttributeMenuWidgetController))
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		
		// 绑定属性变化的回调
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController;
}

void AAuraHUD::InitAttributeDebugPanel(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AS);
	if (!IsValid(ASC) || !IsValid(AuraAttributeSet))
	{
		return;
	}

	if (!IsValid(AttributeDebugWidget))
	{
		UClass* WidgetClass = AttributeDebugWidgetClass
			? AttributeDebugWidgetClass.Get()
			: UAuraAttributeDebugWidget::StaticClass();

		AttributeDebugWidget = CreateWidget<UAuraAttributeDebugWidget>(GetOwningPlayerController(), WidgetClass);
		if (!IsValid(AttributeDebugWidget))
		{
			return;
		}

		AttributeDebugWidget->AddToViewport(1000);
	}

	AttributeDebugWidget->InitializeWithAbilitySystem(ASC, AuraAttributeSet);
}
