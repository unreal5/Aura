// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class APlayerState;
class APlayerController;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	// C++特性：如果有构造函数，不会自动生成默认构造函数，所以需要显式定义一个默认构造函数，以便在蓝图中使用。
	FWidgetControllerParams() = default;
	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState,
	                        UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet;
};

UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Aura | WidgetController")
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	virtual void BroadcastInitialValues()
	{
	}

	virtual void BindCallbacksToDependencies()
	{
	}

protected:
	// WidgetController需要知道PlayerController，PlayerState，AbilitySystemComponent，AttributeSet等信息来更新UI。
	UPROPERTY(BlueprintReadOnly, Category = "Aura | WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category = "Aura | WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly, Category = "Aura | WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Aura | WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
