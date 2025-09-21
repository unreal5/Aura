// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilityBlueprint.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "InterAction/EnemyInterface.h"
#include "Tag/GlobalTag.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	//PathSpline->SetupAttachment(GetRootComponent());
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController()) return;
	// 输入和UI交互方式只与本地玩家相关
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

	auto AuraInputComp = CastChecked<UAuraInputComponent>(InputComponent);

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
	AuraInputComp->BindActionValueLambda(MoveAction, ETriggerEvent::Triggered, MoveFunctor);

	// 绑定能力输入
	AuraInputComp->BindAbilityActions(InputConfig, this,
	                                  &ThisClass::AbilityInputTagPressed,
	                                  &ThisClass::AbilityInputTagReleased,
	                                  &ThisClass::AbilityInputTagHeld);
	//AuraInputComp->BindAction(InputConfig->AbilityInputActions[0].InputAction, ETriggerEvent::Started, this, &ThisClass::AbilityInputTagPressed, InputConfig->AbilityInputActions[0].InputTag);
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

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(GlobalTag::InputTag_LMB))
	{
		bTargeting = ThisActor && ThisActor->Implements<UEnemyInterface>();
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (auto ASC = GetASC())
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(GlobalTag::InputTag_LMB))
	{
		if (auto ASC = GetASC())
		{
			ASC->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if (bTargeting)
	{
		if (auto ASC = GetASC())
		{
			ASC->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility,false,Hit))
		{
			CachedDestionation = Hit.ImpactPoint;
		}
		if (auto ControlledPawn = GetPawn<APawn>())
		{
			const FVector WorldDirection = (CachedDestionation - ControlledPawn->GetActorLocation()).GetSafeNormal2D();
			ControlledPawn->AddMovementInput(WorldDirection,1.f);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (nullptr == AuraASC)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AuraASC;
}
