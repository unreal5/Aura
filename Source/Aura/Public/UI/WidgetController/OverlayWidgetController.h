// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeValueChangedWithTagSignature, FGameplayTag, AttributeTag,
                                             float, NewValue);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	void BroadcastInitialValues() override;
	void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category = "Aura | Delegate")
	FOnAttributeValueChangedWithTagSignature OnAttributeChangedWithTag;
};
