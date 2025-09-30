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
	if (!PC) return nullptr;

	auto AuraHud = PC->GetHUD<AAuraHUD>();
	if (!AuraHud) return nullptr;

	auto PS = PC->GetPlayerState<AAuraPlayerState>();
	if (!PS) return nullptr;
	auto ASC = PS->GetAbilitySystemComponent();
	if (!ASC) return nullptr;
	auto AS = PS->GetAttributeSet();
	if (!AS) return nullptr;

	const FWidgetControllerParameters Params{PC, PS, ASC, AS};
	return AuraHud->GetOverlayWidgetController(Params);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC) return nullptr;

	auto AuraHud = PC->GetHUD<AAuraHUD>();
	if (!AuraHud) return nullptr;

	auto PS = PC->GetPlayerState<AAuraPlayerState>();
	if (!PS) return nullptr;
	auto ASC = PS->GetAbilitySystemComponent();
	if (!ASC) return nullptr;
	auto AS = PS->GetAttributeSet();
	if (!AS) return nullptr;

	const FWidgetControllerParameters Params{PC, PS, ASC, AS};
	return AuraHud->GetAttributeMenuWidgetController(Params);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                            ECharacterClass CharacterClass, float Level,
                                                            UAbilitySystemComponent* ASC)
{
	auto AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGM) return;
	if (!AuraGM->CharacterClassInfo) return;
	if (!ASC) return;

	auto ClassDefaultInfo = AuraGM->CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	auto ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(ASC->GetAvatarActor());

	auto PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

	auto SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(AuraGM->CharacterClassInfo->SecondaryAttributes, Level,
	                                                           ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);
	auto VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(AuraGM->CharacterClassInfo->VitalAttributes, Level,
	                                                       ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	auto AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGM) return;
	if (!AuraGM->CharacterClassInfo) return;
	if (!ASC) return;

	for (auto& CommonAbility : AuraGM->CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(CommonAbility, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}
