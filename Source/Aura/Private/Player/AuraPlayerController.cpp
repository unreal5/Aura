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

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
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

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

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

void AAuraPlayerController::CursorTrace()
{
	if (!GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
	{
		// 导航和释放技能等都依赖Hit，因此如果没有有效的Hit，考虑设置一个默认值。
		if (auto ControlledPawn = GetPawn())
		{
			CursorHit.ImpactPoint = ControlledPawn->GetActorLocation();
		}
		return;
	}

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
	check(GetASC());
	GetASC()->AbilityInputTagReleased(InputTag);

	// 如果使用这种简单的方式，客户端抖动。Topdown示例却不抖动，why？
	// if (InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB))
	// {
	// 	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
	// }
	FollowTime = 0.f;
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 对鼠标左键特殊处理
	if (!InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB))
	{
		if (nullptr != GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	// 按下鼠标左键
	if (bTargeting)
	{
		if (nullptr != GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		CachedDestination = CursorHit.ImpactPoint;
		// Move towards mouse pointer or touch

		if (APawn* ControlledPawn = GetPawn())
		{
			FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
		}
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
