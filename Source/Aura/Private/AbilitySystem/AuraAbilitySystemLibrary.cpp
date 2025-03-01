// 版权没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "HUD/AuraHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// Widget 只可能在LocalPlayer上存在
	if (auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (auto AuraHud = PC->GetHUD<AAuraHUD>())
		{
			auto AuraPlayerState = PC->GetPlayerState<AAuraPlayerState>();
			auto ASC = AuraPlayerState->GetAbilitySystemComponent();
			auto AS = AuraPlayerState->GetAttributeSet();
			FWidgetControllerParams Params{PC, AuraPlayerState, ASC, AS};
			return AuraHud->GetOverlayWidgetController(Params);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (auto AuraHud = PC->GetHUD<AAuraHUD>())
		{
			auto AuraPlayerState = PC->GetPlayerState<AAuraPlayerState>();
			auto ASC = AuraPlayerState->GetAbilitySystemComponent();
			auto AS = AuraPlayerState->GetAttributeSet();
			FWidgetControllerParams Params{PC, AuraPlayerState, ASC, AS};
			return AuraHud->GetAttributeMenuWidgetController(Params);
		}
	}
	return nullptr;
}
