// 版本没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemGlobals.h"

FAuraGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext();
}
