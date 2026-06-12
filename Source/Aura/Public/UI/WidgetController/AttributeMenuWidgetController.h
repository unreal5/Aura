// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class UAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegate, const FAuraAttributeInfo&, AttributeInfo);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	void BroadcastInitialValues() override;
	void BindCallbacksToDependencies() override;
	void WidgetControllerParametersChanged(const FWidgetControllerParams& WCParams) override;
	
	UPROPERTY(BlueprintAssignable, Category = "Aura | AttributeMenuWidgetController")
	FAttributeInfoDelegate AttributeInfoDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura | AttributeMenuWidgetController")
	TObjectPtr<UAttributeInfo> AttributeInfo;
};