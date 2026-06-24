#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/* 不要忘记在.h文件中包含CommonTypes.generated.h，否则会报错 */
#include "AuraCommonTypes.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeValueChangedWithTagSignature, FGameplayTag, AttributeTag, float, NewValue);

// 委托 要按如下格式声明，不能直接使用DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams宏，否则不会生成蓝图可用的委托类型
// 即：不能生成generated.h头文件中所需的代码，导致蓝图中无法使用该委托类型。
UDELEGATE(BlueprintCallable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnAttributeValueChangedWithTagSignature,
	FGameplayTag, AttributeTag,
	float, NewValue
);