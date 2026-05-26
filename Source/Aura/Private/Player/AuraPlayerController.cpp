// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/HitResult.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Pawn.h"
#include "Interaction/HighlightInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
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

	auto EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))
	{
		return;
	}
	// Bind Move Input Action
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
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
	if (!HitResult.bBlockingHit) return;

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
}
