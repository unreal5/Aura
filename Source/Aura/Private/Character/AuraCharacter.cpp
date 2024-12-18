// 版本没有，拷贝自由。


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	// 约束在平面上移动
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// 客户端：初始化ASC
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

// 服务器：初始化ASC
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	// 只在服务端初始化即可，客户端自动同步。
	// 也可以在客户端初始化，这样体验更好。
	// 但客户端初始化后，会被同步到客户端的数据覆写。
	// 如果同时在服务端和客户端初始化，可以在InitAbilityActorInfo中进行，该函数会在服务端和客户端调用。
	InitializeDefaultAttributes();

	// GameplayAbility的初始化只在服务端进行
	AddCharacterAbilities();
}

int32 AAuraCharacter::GetPlayerLevel() const
{
	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	
	check(AuraPlayerState);
	
	if(AuraPlayerState) return AuraPlayerState->GetPlayerLevel();
	return Super::GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	/*
		const uint32 CurrentThreadId = FPlatformTLS::GetCurrentThreadId();
		FString ASCIsNull = AbilitySystemComponent == nullptr ? "null" : "not null";
		FString IsServer = HasAuthority() ? "Server" : "Client";
		UE_LOG(LogTemp, Warning, TEXT("ThreadID: %d, ASCIsNull: %s, IsServer = %s"), CurrentThreadId, *ASCIsNull,
		       *IsServer);
	*/

	// 此时还没有初始化AbilitySystemComponent
	check(AbilitySystemComponent == nullptr);

	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	check(AbilitySystemComponent != nullptr);

	auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (AuraASC)
	{
		AuraASC->AbilityActorInfoSet();
	}
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	AttributeSet = AuraPlayerState->GetAttributeSet();
	check(AttributeSet != nullptr);

	AAuraPlayerController* AuraPlayerController = GetController<AAuraPlayerController>();
	// 客户端只有本机控制的角色才有Controller，模拟的也会调用OnRep_PlayerState（每个玩家的PlayerState均复制），但是没有Controller
	if (AuraPlayerController != nullptr)
	{
		// 只需记住一个准则：本机玩家才有HUD
		AAuraHUD* AuraHUD = AuraPlayerController->GetHUD<AAuraHUD>();
		// 服务器存在没有HUD的情况，例如dedicated server，都没有HUD；对于listen server，只有玩家控制的角色才有HUD，其他角色没有HUD
		if (AuraHUD != nullptr)
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
