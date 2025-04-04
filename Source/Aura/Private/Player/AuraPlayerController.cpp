// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SplineComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"


#include "Interaction/EnemyInterface.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		FVector CurrentLocation = ControlledPawn->GetActorLocation();
		FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(
			CurrentLocation, ESplineCoordinateSpace::World);
		FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction, 1.0, false);

		const float Distance = FVector::Dist(LocationOnSpline, CachedDestination);
		bAutoRunning = (Distance > AutoRunAcceptanceRadius);
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
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

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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

void AAuraPlayerController::ShiftPressed()
{
	bShiftKeyDown = true;
}

void AAuraPlayerController::ShiftReleased()
{
	bShiftKeyDown = false;
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB))
	{
		// 如果按下时，CurrentActor非空，则我们可能是释放技能/移动
		bTargeting = CurrentActor != nullptr;
		// 按下时，如果是移动，则需要停止当前的移动，更改移动目的地。
		// 只有在释放按下的鼠标时，才能判断是长按还是短按。
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	// 对鼠标左键特殊处理
	if (!InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB) || bTargeting || bShiftKeyDown)
	{
		return;
	}

	auto ControlledPawn = GetPawn();
	if (FollowTime <= ShortPreesThreshold && ControlledPawn != nullptr) // 短按
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
			this, ControlledPawn->GetActorLocation(), CachedDestination))
		{
			SplineComponent->ClearSplinePoints();
			for (const auto& PointLoc : NavPath->PathPoints)
			{
				SplineComponent->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
			}
			if (NavPath->PathPoints.Num() > 0)
			{
				auto LastPoint = NavPath->PathPoints.Last();
				CachedDestination = LastPoint;
				bAutoRunning = true;
			}
		}
	}

	FollowTime = 0.f;
	bTargeting = false;
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 对鼠标左键特殊处理
	if (!InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB) || bTargeting || bShiftKeyDown)
	{
		if (nullptr != GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	FollowTime += GetWorld()->GetDeltaSeconds();
	if (CursorHit.bBlockingHit)
	{
		CachedDestination = CursorHit.ImpactPoint;
	}
	// Move towards mouse pointer or touch
	if (APawn* ControlledPawn = GetPawn())
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AuraAbilitySystemComponent)
	{
		auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(ASC);
	}
	return AuraAbilitySystemComponent;
}
