// 版权没有，拷贝自由。


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 800.f;
	SpringArm->SetRelativeLocation(FVector{0.f, 0.f, 100.f});
	SpringArm->SetRelativeRotation(FRotator{-45.f, 0.f, 0.f});
	// bUsePawnControlRotation=false，CameraBoom不会跟随玩家的控制器进行pitch操作。
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag = true;
	// 固定摄像机位置，不允许旋转
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// 角色自身不允许跟随控制器旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 角色朝向跟随移动方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator{0.f, 720.f, 0.f}; // 每秒旋转720度
	/* 以下是典型的TopDown风格的角色移动设置 */
	// 强制在平面移动
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
}

// 客户端调用
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

// 服务器端调用
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	AttributeSet = AuraPlayerState->GetAttributeSet();
	check(AttributeSet);

	// 初始化HUD。InitAbilityActorInfo会被客户端OnRep_PlayerState()调用多次（每个玩家状态都会复制）
	// 而HUD只有本机玩家（local player)才能看到，所以只在本机玩家调用
	if (IsLocallyControlled())
	{
		APlayerController* PC = GetController<APlayerController>();
		check(PC);

		if (auto AuraHUD = PC->GetHUD<AAuraHUD>())
		{
			AuraHUD->InitOverlay(PC, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
