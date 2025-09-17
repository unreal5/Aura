// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

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
