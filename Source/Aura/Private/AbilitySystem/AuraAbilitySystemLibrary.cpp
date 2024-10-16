// 版本没有，拷贝自由。


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(UObject* WorldContextObject)
{
	auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC == nullptr)
	{
		return nullptr;
	}

	auto AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (AuraHUD == nullptr)
	{
		return nullptr;
	}
	auto AuraPlayerState = PC->GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState == nullptr)
	{
		return nullptr;
	}

	auto ASC = AuraPlayerState->GetAbilitySystemComponent();
	auto AS = AuraPlayerState->GetAttributeSet();

	const FWidgetControllerParams Params(PC, AuraPlayerState, ASC, AS);
	return AuraHUD->GetOverlayWidgetController(Params);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WorldContextObject)
{
	auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC == nullptr)
	{
		return nullptr;
	}

	auto AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (AuraHUD == nullptr)
	{
		return nullptr;
	}
	auto AuraPlayerState = PC->GetPlayerState<AAuraPlayerState>();
	if (AuraPlayerState == nullptr)
	{
		return nullptr;
	}

	auto ASC = AuraPlayerState->GetAbilitySystemComponent();
	auto AS = AuraPlayerState->GetAttributeSet();

	const FWidgetControllerParams Params(PC, AuraPlayerState, ASC, AS);
	return AuraHUD->GetAttributeMenuWidgetController(Params);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(UObject* WorldContextObject, ECharacterClass CharacterClass,
                                                            float Level, UAbilitySystemComponent* ASC)
{
	auto CharacterClassInfo = GetCharacterClassInfo(WorldContextObject); //AuraGameMode->CharacterClassInfo.Get();
	if (CharacterClassInfo == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();


	auto EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AvatarActor);

	auto ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	// 主属性
	auto PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level,
	                                                         EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);
	// 次属性
	auto SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level,
	                                                           EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);
	// 重要属性
	auto VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level,
	                                                       EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(UObject* WorldContextObject, UAbilitySystemComponent* ASC,
                                                     ECharacterClass CharacterClass)
{
	//auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	//if (AuraGameMode == nullptr) return;

	auto CharacterClassInfo = GetCharacterClassInfo(WorldContextObject); //AuraGameMode->CharacterClassInfo.Get();
	if (CharacterClassInfo == nullptr) return;


	for (auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		// 这些技能不会随着角色等级提升
		auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	// 获取PlayerLevel
	auto CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor());
	const auto PlayerLevel = CombatInterface ? CombatInterface->GetPlayerLevel() : 1;
	const auto& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	for (auto AbilityClass : DefaultInfo.StartupAbilities)
	{
		auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, PlayerLevel);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(UObject* WorldContextObject)
{
	auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;

	return AuraGameMode->CharacterClassInfo.Get();
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	auto AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return nullptr != AuraContext ? AuraContext->IsBlockedHit() : false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit)
{
	auto AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (nullptr != AuraContext)
	{
		AuraContext->SetIsBlockedHit(bIsBlockedHit);
	}
}


bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	auto AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	return nullptr != AuraContext ? AuraContext->IsCriticalHit() : false;
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit)
{
	auto AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (nullptr != AuraContext)
	{
		AuraContext->SetIsCriticalHit(bIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors,
                                                           const TArray<AActor*>& IgnoreActors, const FVector& Origin,
                                                           float Radius)
{
	FCollisionQueryParams SphereParams;

	SphereParams.AddIgnoredActors(IgnoreActors);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity,
		                                FCollisionObjectQueryParams(
			                                FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                                FCollisionShape::MakeSphere(Radius), SphereParams);

		for (auto& Ovelap : Overlaps)
		{
			auto Actor = Ovelap.GetActor();
			if (Actor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Actor))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Actor));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriendly(const AActor* ActorA, const AActor* ActorB)
{
	auto AIsPlayer= ActorA->ActorHasTag(FName("Player"));
	auto BIsPlayer = ActorB->ActorHasTag(FName("Player"));
	return AIsPlayer != BIsPlayer;
}