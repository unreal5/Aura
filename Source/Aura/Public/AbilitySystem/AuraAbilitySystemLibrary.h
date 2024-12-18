// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library | WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library | WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="Aura Ability System Library | CharacterClassDefaults")
	static void InitializeDefaultAttributes(UObject* WorldContextObject, ECharacterClass CharacterClass, float Level,
	                                        UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="Aura Ability System Library | CharacterClassDefaults")
	static void GiveStartupAbilities(UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="Aura Ability System Library | CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="Aura Ability System Library | GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura Ability System Library | GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit);
	
	UFUNCTION(BlueprintPure, Category="Aura Ability System Library | GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura Ability System Library | GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category="Aura Ability System Library | GameplayMechanics")
	static void GetLivePlayersWithinRadius(UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& IgnoreActors, const FVector& Origin, float Radius);

	UFUNCTION(BlueprintPure, Category="Aura Ability System Library | GameplayMechanics")
	static bool IsNotFriendly(const AActor* ActorA, const AActor* ActorB); 
};