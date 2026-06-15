// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Character/Enemy/AuraEnemy.h"
#include "Components/SplineComponent.h"
#include "Engine/HitResult.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/HighlightInterface.h"
#include "Tag/AuraGlobalTags.h"
#include "UI/HUD/AuraHUD.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(GetRootComponent());
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 非本地控制器不需要设置输入映射和鼠标光标
	if (!IsLocalController())
		return;

	auto LocalPlayer = GetLocalPlayer();
	if (!IsValid(LocalPlayer))
		return;

	if (auto InputSubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		InputSubSystem->AddMappingContext(AuraMappingContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto EnhancedInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		return;
	}
	// Bind Move Input Action
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Started, this,
	                                   &ThisClass::ToggleAttributeDebugPanel);

	EnhancedInputComponent->BindAbilityAction(AuraInputConfig, this, &ThisClass::AbilityInputPressed,
	                                          &ThisClass::AbilityInputReleased, &ThisClass::AbilityInputHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	if (MovementVector.IsZero()) return;

	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;

	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	FVector ForwardDirection = FVector{MovementVector.Y, MovementVector.X, 0};
	ForwardDirection = YawRotation.RotateVector(ForwardDirection);
	ControlledPawn->AddMovementInput(ForwardDirection);
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	if (!HitResult.bBlockingHit)
	{
		CachedDestination = FVector::ZeroVector;
		return;
	}

	CurrentActor = HitResult.GetActor();
	if (LastActor != CurrentActor)
	{
		if (IsValid(LastActor) && LastActor->Implements<UHighlightInterface>())
		{
			IHighlightInterface::Execute_UnHighlightActor(LastActor);
		}

		if (IsValid(CurrentActor) && CurrentActor->Implements<UHighlightInterface>())
		{
			IHighlightInterface::Execute_HighlightActor(CurrentActor);
		}
	}
	LastActor = CurrentActor;

	// 更新目标位置
	CachedDestination = HitResult.ImpactPoint;
}

void AAuraPlayerController::ToggleAttributeDebugPanel()
{
	if (!IsLocalController())
	{
		return;
	}

	if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(GetHUD()))
	{
		AuraHUD->ToggleAttributeDebugPanel();
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!IsValid(CachedAuraASC))
	{
		CachedAuraASC = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return CachedAuraASC;
}

void AAuraPlayerController::AbilityInputPressed(const FGameplayTag InputTag)
{
	if (InputTag == InputAction::LMB)
	{
		bTargeting = CurrentActor && CurrentActor->IsA(AAuraEnemy::StaticClass());
		// 取消自动移动
		bAutoRunning = false;
	}
	FollowTime = 0.0f;
}

void AAuraPlayerController::AbilityInputReleased(const FGameplayTag InputTag)
{
	auto ASC = GetASC();
	if (!IsValid(ASC)) return;

	ASC->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputHeld(const FGameplayTag InputTag)
{
	auto ASC = GetASC();
	if (!IsValid(ASC)) return;

	if (bTargeting || !InputTag.MatchesTagExact(InputAction::LMB))
	{
		ASC->AbilityInputTagHeld(InputTag);
	}
	else // 如果是左键，并且当前没有目标，则进行自动移动
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		// 在PlayerTick中已经通过CursorTrace不断更新了CachedDestination，这里不需要再次获取鼠标位置。
		
		if (APawn* ControlledPawn = GetPawn())
		{
			ControlledPawn->AddMovementInput((CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal());
		}
	}
}