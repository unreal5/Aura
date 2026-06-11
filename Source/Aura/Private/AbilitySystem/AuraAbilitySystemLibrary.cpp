// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

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
