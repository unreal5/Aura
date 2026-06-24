// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC)
	{
		if (auto AuraHud = Cast<AAuraHUD>(PC->GetHUD()))
		{
			auto PS = PC->GetPlayerState<AAuraPlayerState>();
			auto ASC = PS ? PS->GetAbilitySystemComponent() : nullptr;
			auto AS = PS ? PS->GetAttributeSet() : nullptr;
			FWidgetControllerParams Params(PC, PS, ASC, AS);
			return AuraHud->GetOverlayWidgetController(Params);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC)
	{
		if (auto AuraHud = Cast<AAuraHUD>(PC->GetHUD()))
		{
			auto PS = PC->GetPlayerState<AAuraPlayerState>();
			auto ASC = PS ? PS->GetAbilitySystemComponent() : nullptr;
			auto AS = PS ? PS->GetAttributeSet() : nullptr;
			FWidgetControllerParams Params(PC, PS, ASC, AS);
			return AuraHud->GetAttributeMenuWidgetController(Params);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                            ECharacterClass InCharacterClass, float InLevel,
                                                            UAbilitySystemComponent* InASC)
{
	if (!InASC) return;
	auto Avatar = InASC->GetAvatarActor();
	if (!Avatar) return;
	
	auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGameMode) return;

	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	check(CharacterClassInfo);
	
	auto ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(InCharacterClass);
	
	auto ContextHandle = InASC->MakeEffectContext();
	ContextHandle.AddSourceObject(Avatar);
	
	auto PrimaryAttributeSpecHandle = InASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, InLevel, ContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data);
	
	// Secondary
	auto SecondaryAttributeSpecHandle = InASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, InLevel, ContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data);
	
	// Vital
	auto VitalAttributeSpecHandle = InASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, InLevel, ContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data);
}