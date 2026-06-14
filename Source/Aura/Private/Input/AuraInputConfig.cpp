// 版权没有，拷贝自由。


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::GetInputAction(const FGameplayTag& InputTag) const
{
	for (const FAuraInputAction& Action : Actions)
	{
		if (Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}
