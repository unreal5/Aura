// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayTag;
struct FGameplayAttribute;
class UAttributeInfo;
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoDelegate, const FAuraAttributeInfo&, AttributeInfo);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|AttributeMenu")
	FAttributeInfoDelegate AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
private:
	void BroadcastAttributeInfo(const FGameplayTag& InTag, const FGameplayAttribute& InAttribute) const;
};
