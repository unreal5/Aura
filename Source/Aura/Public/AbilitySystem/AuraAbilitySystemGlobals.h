// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilityTypes.h"
#include "AuraAbilitySystemGlobals.generated.h"


UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FAuraGameplayEffectContext* AllocGameplayEffectContext() const override;
};
