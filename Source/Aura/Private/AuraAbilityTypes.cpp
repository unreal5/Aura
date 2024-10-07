#include "AuraAbilityTypes.h"

UScriptStruct* FAuraGameplayEffectContext::GetScriptStruct() const
{
	return StaticStruct();
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	bool bRet = FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bIsBlockedHit) RepBits |= 1 << 0;
		if (bIsCriticalHit) RepBits |= 1 << 1;
	}
	Ar.SerializeBits(&RepBits, 2);
	if (Ar.IsLoading())
	{
		bIsBlockedHit = RepBits & (1 << 0);
		bIsCriticalHit = RepBits & (1 << 1);
	}
	return bRet;
}
