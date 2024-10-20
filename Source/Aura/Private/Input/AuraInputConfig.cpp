// 版本没有，拷贝自由。


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for(auto& AbilityInputAction : AbilityInputActions)
	{
		if(AbilityInputAction.InputTag.MatchesTagExact(InputTag))
		{
			return AbilityInputAction.InputAction.Get();
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("No input action found for tag %s"), *InputTag.ToString());
	}
	return nullptr;
}
