// 版权没有，拷贝自由。


#include "Character/AuraCharacter.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/AuraHUD.h"
#include "MotionWarpingComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

AAuraCharacter::AAuraCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 700.0f; // The camera follows at this distance behind the character
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	// 对于Topdown游戏，要限制角色只能在XY平面移动。
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Motion Warping
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}


void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 只处理客户端，服务端由PossessedBy()处理。
	check(!HasAuthority());

	InitAbilityActorInfo();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 只处理服务端，客户端由OnRep_PlayerState()处理。
	check(HasAuthority());

	InitAbilityActorInfo();

	AddCharacterAbilities();
}

int32 AAuraCharacter::GetPlayerLevel() const
{
	if (auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		return AuraPS->GetPlayerLevel();
	}
	checkf(0, TEXT("可能过早调用"))
	return 0;
}



void AAuraCharacter::UpdateFacingTarget_Implementation(const FVector& TargetLocation)
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("FacingTarget"), TargetLocation);
}

void AAuraCharacter::InitAbilityActorInfo()
{
	auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	if (!AuraPlayerState) return;

	// 初始化技能组件和属性集。
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	check(AbilitySystemComponent);
	if (!AbilitySystemComponent) return;

	AttributeSet = AuraPlayerState->GetAttributeSet();
	check(AttributeSet);

	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	if (auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->AbilityActorInfoSet();
	}
	// 初始化HUD
	if (IsLocallyControlled())
	{
		if (auto PC = GetController<AAuraPlayerController>())
		{
			if (auto HUD = PC->GetHUD<AAuraHUD>())
			{
				HUD->InitOverlay(PC, AuraPlayerState, AbilitySystemComponent, AttributeSet);
			}
			else
			{
				checkf(0, TEXT("这种情况不应该出现（已经使用IsLocallyControlled()检查是否本机）。"));
			}
		}
	}

	//  服务端初始化属性,客户端不需要初始化属性，因为客户端会在OnRep_PlayerState()中复制
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}
