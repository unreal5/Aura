// 版权没有，拷贝自由。


#include "UI/Widget/Overlay/AuraWidgetOverlay.h"
#include "UI/Widget/Overlay/SubWidget/EffectMessage.h"
#include "UI/Widget/ProgressBar/GlobeProgressBar.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UAuraWidgetOverlay::OnMessageWidgetRow(const FUiWidgetRow& Row)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlay Widget 接收到消息：[%s]"), *Row.Message.ToString());
	auto LocalPlayerController = GetOwningPlayer();
	if (!LocalPlayerController) return;

	auto EffectMessage = CreateWidget<UEffectMessage>(LocalPlayerController, Row.MessageWidget);
	if (!EffectMessage) return;

	int32 Width, Height;
	LocalPlayerController->GetViewportSize(Width, Height);
	EffectMessage->SetPositionInViewport(FVector2D(Width / 2, Height / 2));

	EffectMessage->SetImageAndText(Row.Image, Row.Message);
	EffectMessage->AddToViewport();
}

void UAuraWidgetOverlay::WidgetControllerSet_Implementation()
{
	//Super::WidgetControllerSet_Implementation();
	WBP_HealthGlobe->SetWidgetController(WidgetController);
	WBP_ManaGlobe->SetWidgetController(WidgetController);

	if (auto OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController))
	{
		OverlayWidgetController->MessageWidgetRowDelegate.AddDynamic(this, &UAuraWidgetOverlay::OnMessageWidgetRow);
	}
}
