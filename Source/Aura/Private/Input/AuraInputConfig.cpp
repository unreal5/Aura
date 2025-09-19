// 版权没有，拷贝自由。


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                   bool bLogNotFound) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (!Action.InputTag.MatchesTagExact(InputTag))
			continue;

		return Action.InputAction;
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("No input action found for tag: %s"), *InputTag.ToString());
	}
	return nullptr;
}
