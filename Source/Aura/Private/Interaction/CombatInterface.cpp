// 版权没有，拷贝自由。


#include "Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetPlayerLevel() const
{
	checkf(0, TEXT("没有实现接口函数"));
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation() const
{
	return FVector::Zero();
}
