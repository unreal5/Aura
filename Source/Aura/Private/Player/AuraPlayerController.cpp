// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


#include "Interaction/EnemyInterface.h"

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

	// 非本机控制，设置键盘、鼠标相关的无意义。
	if (!IsLocalController())
		return;
	
	check(AuraContext);

	if (auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext, 0);
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

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	FRotator Rotation{0.f, GetControlRotation().Yaw, 0.f};
	FVector Direction{Movement.Y, Movement.X, 0.f};
	FVector WorldDirection = Rotation.RotateVector(Direction);
	if (auto ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(WorldDirection);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	if (!GetHitResultUnderCursor(ECC_Visibility, false, CursorHit)) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	if (LastActor != CurrentActor)
	{
		if (LastActor)
		{
			LastActor->UnhighlightActor();
		}
		if (CurrentActor)
		{
			CurrentActor->HighlightActor();
		}
	}
}
