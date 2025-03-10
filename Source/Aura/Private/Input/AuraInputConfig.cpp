// 版权没有，拷贝自由。


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                   bool bLogNotFound) const
{
	auto Found = AbilityInputActions.FindByPredicate([&InputTag](const FAuraInputAction& Action)
	{
		return InputTag.MatchesTagExact(Action.InputTag);
	});
	if (!Found && bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("No input action found for tag %s"), *InputTag.ToString());
	}
	return Found ? Found->InputAction.Get() : nullptr;
}
