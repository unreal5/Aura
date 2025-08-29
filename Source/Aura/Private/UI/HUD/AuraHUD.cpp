// 版权没有，拷贝自由。


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	if (OverlayWidget)
	{
		OverlayWidget->AddToViewport();
	}
}
