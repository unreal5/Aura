// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataChangedDelegate, float, NewValue);

USTRUCT(Blueprintable, BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories = "Message"))
	FGameplayTag MessageTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, MessageRow);

/*
 * Overlay Widget 的控制器
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnDataChangedDelegate OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnDataChangedDelegate OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnDataChangedDelegate OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnDataChangedDelegate OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS | 消息")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename RowType>
	RowType* GetDataTableRowByTag(UDataTable* InDataTable, const FGameplayTag& InTag) const;
};

template <typename RowType>
RowType* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* InDataTable, const FGameplayTag& InTag) const
{
	if (!InDataTable) return nullptr;

	return InDataTable->FindRow<RowType>(InTag.GetTagName(), "");
}
