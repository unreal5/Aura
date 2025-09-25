// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
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

	if (bAutoRunning)
		AutoRun();
}

void AAuraPlayerController::AutoRun()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	const int32 NumPoints = PathSpline->GetNumberOfSplinePoints();
	if (NumPoints == 0)
	{
		bAutoRunning = false;
		return;
	}

	// Pawn不一定在样条线上，所以找到样条线离Pawn最近的点
	const auto LocationOnSpline = PathSpline->FindLocationClosestToWorldLocation(
		ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	const auto Direction = PathSpline->FindDirectionClosestToWorldLocation(
		LocationOnSpline, ESplineCoordinateSpace::World);
	ControlledPawn->AddMovementInput(Direction);

	const float DistanceToDestination = (LocationOnSpline - CachedDestionation).Length();
	if (DistanceToDestination <= AutoRunAcceptanceRadius)
	{
		bAutoRunning = false;
		PathSpline->ClearSplinePoints();
	}
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

	// Shift 按下
	AuraInputComp->BindActionValueLambda(ShiftAction, ETriggerEvent::Started, [this](auto/* Make UE happy! */) { bShiftKeyDown = true; });
	// Shift 松开
	AuraInputComp->BindActionValueLambda(ShiftAction, ETriggerEvent::Completed, [this](auto/* Make UE happy! */) { bShiftKeyDown = false; });

	// 绑定能力输入
	AuraInputComp->BindAbilityActions(InputConfig, this,
	                                  &ThisClass::AbilityInputTagPressed,
	                                  &ThisClass::AbilityInputTagReleased,
	                                  &ThisClass::AbilityInputTagHeld);
	//AuraInputComp->BindAction(InputConfig->AbilityInputActions[0].InputAction, ETriggerEvent::Started, this, &ThisClass::AbilityInputTagPressed, InputConfig->AbilityInputActions[0].InputTag);
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);

	if (!CursorHitResult.bBlockingHit) return;

	// 基本算法是记录当前高亮和前一帧高亮的敌人，如果不同则取消前一帧的高亮，如果相同则不操作
	// 对于相同的情况，如果都是空则不操作（一个有趣的设计原则：假设前面做的都是正确的，那么就不需要再做一次）
	IEnemyInterface* HitEnemy = Cast<IEnemyInterface>(CursorHitResult.GetActor());
	if (HitEnemy == LastFrameHitEnemy) return;

	// 不相等的情况，取消前一帧的高亮，高亮当前帧。
	if (LastFrameHitEnemy)
	{
		LastFrameHitEnemy->UnHighlightActor();
	}
	if (HitEnemy)
	{
		HitEnemy->HighlightActor();
	}
	LastFrameHitEnemy = HitEnemy;
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(GlobalTag::InputTag_LMB))
	{
		bTargeting = LastFrameHitEnemy != nullptr;
		bAutoRunning = false;
	}
	FollowTime = 0.f;
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	auto ASC = GetASC();
	if (nullptr == ASC) return;

	ASC->AbilityInputTagReleased(InputTag);
	
	if (!InputTag.MatchesTagExact(GlobalTag::InputTag_LMB))
		return;
		
	
	if (!bTargeting && !bShiftKeyDown)
	{
		APawn* ControlledPawn = GetPawn();
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (FollowTime < ShortPressThreshold && ControlledPawn)
		{
			// 短按，点击移动
			auto NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestionation);
			if (NavPath)
			{
				PathSpline->ClearSplinePoints();
				for (const auto PointLoc : NavPath->PathPoints)
				{
					PathSpline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					// DrawDebugSphere(GetWorld(), PointLoc, 10.f, 12, FColor::MakeRandomColor(), false, 5.f);
				}
				if (NavPath->PathPoints.Num() > 0)
					CachedDestionation = NavPath->PathPoints.Last();
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	auto ASC = GetASC();
	if (nullptr == ASC) return;


	// 非左键，直接传递
	if (!InputTag.MatchesTagExact(GlobalTag::InputTag_LMB))
	{
		ASC->AbilityInputTagHeld(InputTag);
		return;
	}
	// 左键
	if (bTargeting || bShiftKeyDown)
	{
		ASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHitResult.bBlockingHit)
		{
			CachedDestionation = CursorHitResult.ImpactPoint;
		}
		if (auto ControlledPawn = GetPawn<APawn>())
		{
			const FVector WorldDirection = (CachedDestionation - ControlledPawn->GetActorLocation()).GetSafeNormal2D();
			ControlledPawn->AddMovementInput(WorldDirection, 1.f);
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
