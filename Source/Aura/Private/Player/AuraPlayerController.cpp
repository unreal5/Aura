// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (AuraContext)
	{
		if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(AuraContext, 0);
		}
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

	// 地面移动
	auto MoveFunctor = [this](const FInputActionValue& Value)
	{
		FVector2D MoveValue = Value.Get<FVector2D>();
		float MoveDist = MoveValue.Size();
		if (FMath::IsNearlyZero(MoveDist)) return;
		if (auto ControlledPawn = GetPawn<APawn>())
		{
			FRotator ControlRot = GetControlRotation();
			ControlRot.Pitch = 0.f;
			ControlRot.Roll = 0.f;
			FVector MoveDirection = FVector{MoveValue.Y, MoveValue.X, 0.f}.GetClampedToMaxSize(1.f);
			MoveDirection = ControlRot.RotateVector(MoveDirection);
			ControlledPawn->AddMovementInput(MoveDirection, MoveDist);
		}
	};
	EnhancedInputComponent->BindActionValueLambda(MoveAction, ETriggerEvent::Triggered, MoveFunctor);
}
