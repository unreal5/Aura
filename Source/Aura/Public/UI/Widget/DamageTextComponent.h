// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageTextComponent();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamagetText(float DamageAmount);
};
