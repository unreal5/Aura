// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;

/* 保存AssetTag <=> Widget消息的映射 */
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, const FUIWidgetRow&, Row);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attribute")
	FOnAttributeValueChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attribute")
	FOnAttributeValueChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attribute")
	FOnAttributeValueChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attribute")
	FOnAttributeValueChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Message")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;


	template <typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	if (!DataTable) return nullptr;

	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
