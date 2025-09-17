// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParameters
{
	GENERATED_BODY()

	FWidgetControllerParameters() = default;

	FWidgetControllerParameters(APlayerController* InPlayerController, APlayerState* InPlayerState,
	                            UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController), PlayerState(InPlayerState),
		  AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState = nullptr;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
/**
 * Controller 负责处理逻辑，目前关注以下类型
 * ASC,AttributeSet,PlayerState及PlayerController
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParameters(const FWidgetControllerParameters& InParameters);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() {}
	virtual void BindCallbacksToDependencies() {}
protected:
	// 从以下依赖中收集数据
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
