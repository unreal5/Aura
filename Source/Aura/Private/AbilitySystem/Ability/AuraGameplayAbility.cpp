// 版权没有，拷贝自由。


#include "AbilitySystem/Ability/AuraGameplayAbility.h"

void UAuraGameplayAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	// 测试代码，输出技能被哪个输入标签激活了
	// UE_LOG(LogTemp, Warning, TEXT("Ability %s is activated by input tag %s"), *GetName(), *StartupInputTag.ToString());
}