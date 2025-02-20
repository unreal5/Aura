// 版权没有，拷贝自由。

#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"



AAuraPlayerState::AAuraPlayerState()
{
	SetNetUpdateFrequency(100.f);
	SetMinNetUpdateFrequency(60.f);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("技能组件"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("属性集"));
}
