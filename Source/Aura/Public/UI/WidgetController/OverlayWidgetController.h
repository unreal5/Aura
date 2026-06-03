// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Templates/SubclassOf.h"
#include "OverlayWidgetController.generated.h"


class UTexture2D;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeValueChangedWithTagSignature, FGameplayTag, AttributeTag,
                                             float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, const FUIWidgetRow&, WidgetRow);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	void BroadcastInitialValues() override;
	void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "Aura | Delegate")
	FOnAttributeValueChangedWithTagSignature OnAttributeChangedWithTag;

	UPROPERTY(BlueprintAssignable, Category = "Aura | Messages")
	FOnMessageWidgetRowSignature OnMessageWidgetRowDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura | 数据表")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
};

template <typename T>
const T* GetDataTableRowByTag(UDataTable* InDataTable, const FGameplayTag& InTag)
{
	if (!IsValid(InDataTable)) return (const T*)nullptr;

	return InDataTable->FindRow<T>(InTag.GetTagName(), "");
}
