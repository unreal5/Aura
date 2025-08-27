// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InterAction/EnemyInterface.h"
#include "Player/AuraPlayerState.h"

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

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);

	if (!CursorHitResult.bBlockingHit) return;

	// 基本算法是记录当前高亮和前一帧高亮的敌人，如果不同则取消前一帧的高亮，如果相同则不操作
	// 对于相同的情况，如果都是空则不操作（一个有趣的设计原则：假设前面做的都是正确的，那么就不需要再做一次）
	LastActor = ThisActor;
	ThisActor = CursorHitResult.GetActor();
	if (ThisActor == LastActor) return;

	// 不相等的情况，取消前一帧的高亮，高亮当前帧。
	if (LastActor && LastActor->Implements<UEnemyInterface>())
	{
		IEnemyInterface::Execute_UnHighlightActor(LastActor);
	}
	if (ThisActor && ThisActor->Implements<UEnemyInterface>())
	{
		IEnemyInterface::Execute_HighlightActor(ThisActor);
	}
}
