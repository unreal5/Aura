#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetIsCriticalHit(bool IsCriticalHit) { bIsCriticalHit = IsCriticalHit; }

	bool IsBlockedHit() const { return bIsBlockedHit; }
	void SetIsBlockedHit(bool IsBlockedHit) { bIsBlockedHit = IsBlockedHit; }

	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};
