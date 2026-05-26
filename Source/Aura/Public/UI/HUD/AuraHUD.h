// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	// 以OverlayWidgetClass为模板创建一个OverlayWidget，并添加到Viewport
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
};
