// 版权没有，拷贝自由。


#include "Character/Player/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


// Sets default values
AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// Topdown 风格，倾斜一定角度
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 750.0f;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.f;
	// 禁止CameraBoom旋转
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	// 限制角色只能在水平面上移动
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 角色自身不旋转，摄像机旋转
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
}


// 客户端调用
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

// 服务器调用
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	if (auto AuraPlayerState = GetPlayerState<AAuraPlayerState>())
	{
		AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
		AttributeSet = AuraPlayerState->GetAttributeSet();

		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
		}
	}

	// 初始化HUD
	APlayerController* PC = GetController<APlayerController>();
	// 客户端：只有本地玩家才有控制器
	// 服务商：只有本地玩家控制器才有HUD
	if (PC && PC->IsLocalPlayerController())
	{
		if (auto AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AuraHUD->InitOverlay(PC, GetPlayerState(), AbilitySystemComponent, AttributeSet);
		}
	}
}
