// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class APlayerState;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAuraUserWidget;
class UAuraAttributeDebugWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	AAuraHUD();



	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void ToggleAttributeDebugPanel();

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	
private:
	void InitAttributeDebugPanel(UAbilitySystemComponent* ASC, UAttributeSet* AS);
	// 以OverlayWidgetClass为模板创建一个OverlayWidget，并添加到Viewport
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere, Category = "Overlay")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere, Category="Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY(Transient)
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category="Debug")
	TSubclassOf<UAuraAttributeDebugWidget> AttributeDebugWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UAuraAttributeDebugWidget> AttributeDebugWidget;
	
	// AttributeMenuWidgetController
	UPROPERTY(EditAnywhere, Category="Overlay")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	UPROPERTY(Transient)
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
};
