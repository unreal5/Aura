// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature, float, NewValue);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	void BroadcastInitialValues() override;
	
	UPROPERTY(BlueprintAssignable, Category = "Aura | Delegate")
	FOnAttributeValueChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Aura | Delegate")
	FOnAttributeValueChangedSignature OnMaxHealthChanged;
	
};
