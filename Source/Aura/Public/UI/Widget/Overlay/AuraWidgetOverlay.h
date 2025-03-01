// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraWidgetOverlay.generated.h"

struct FUiWidgetRow;
class UGlobeProgressBar;
/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetOverlay : public UAuraUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGlobeProgressBar> WBP_HealthGlobe;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UGlobeProgressBar> WBP_ManaGlobe;

private:
	UFUNCTION()
	void OnMessageWidgetRow(const FUiWidgetRow& Row);
	
	virtual void WidgetControllerSet_Implementation() override;
};
