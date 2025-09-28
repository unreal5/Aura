// 版权没有，拷贝自由。


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass) const
{
	return CharacterClassInfo.FindChecked(CharacterClass);
}
