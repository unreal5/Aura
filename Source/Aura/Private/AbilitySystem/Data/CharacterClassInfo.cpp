// 版权没有，拷贝自由。


#include "AbilitySystem/Data/CharacterClassInfo.h"
FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(ECharacterClass CharacterClass) const
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}