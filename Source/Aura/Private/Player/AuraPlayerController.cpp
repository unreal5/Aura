// 版本没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//check(Subsystem);
	// 对于Dedicated Server，Subsystem是空的
	if (Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}


	//鼠标相关
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// 除了硬件输入，还有UI输入
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// Viewport捕获鼠标后不隐藏
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	check(MoveAction);
	// lambda表达式
	auto MoveFunctor = [this](const FInputActionValue& Value)-> void
	{
		// 通过Value来控制移动
		const auto InputAxisVector = Value.Get<FVector2D>();
		// 禁止对角移动比前后左右移动更快
		const auto Distance = FMath::Clamp(InputAxisVector.Size(), 0.f, 1.f);
		FRotator YawRotation{0.f, GetControlRotation().Yaw, 0.f};
		FVector Direction{InputAxisVector.Y, InputAxisVector.X, 0.f};
		Direction = YawRotation.RotateVector(Direction);

		if (auto ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddMovementInput(Direction, Distance);
		}
	};
	AuraInputComponent->BindActionValueLambda(MoveAction, ETriggerEvent::Triggered, MoveFunctor);

	// shift key
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::AutoRun()
{
	if (auto ControlledPawn = GetPawn<APawn>())
	{
		const auto LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const auto Direction = Spline->FindDirectionClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction, 1.f);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	if (bAutoRunning)
	{
		AutoRun();
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,
                                                            bool bBlockedHit, bool bCriticalHit)
{
	if (!::IsValid(TargetCharacter)) return;
	if (!::IsValid(DamageTextComponentClass)) return;

	// 本地玩家才显示伤害数字
	checkf(IsLocalController(), TEXT("不应该在非本地玩家调用ShowDamageNumber"));

	auto DamageTextComp = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
	// 动态创建的组件，需要手动注册，让它快速赶上Owner的节奏
	DamageTextComp->RegisterComponent();
	DamageTextComp->AttachToComponent(TargetCharacter->GetRootComponent(),
	                                  FAttachmentTransformRules::KeepRelativeTransform);

	// 先Attach后Detach，就自动定位
	DamageTextComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	DamageTextComp->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	// 由于DamageTextComponent是临时变量，没有使用UPROPERTY，会被GC回收，不需要过多关注
}


void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
		return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor == ThisActor) return;
	if (ThisActor != nullptr)
	{
		ThisActor->HighlightActor();
	}
	if (LastActor != nullptr)
	{
		LastActor->UnHighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	// 判断点击左键是释放技能还是移动（有无目标）
	if (InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB))
	{
		bTargeting = ThisActor != nullptr;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	if (!InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB) || bTargeting || bShiftKeyDown)
	{
		return;
	}

	// 自动移动？
	bAutoRunning = FollowTime <= ShortPressThreshold;
	auto ControlledPawn = GetPawn<APawn>();
	if (bAutoRunning && ControlledPawn)
	{
		auto NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(),
		                                                                    CachedDestination);
		if (NavPath)
		{
			Spline->ClearSplinePoints();
			for (const auto& PointLoc : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
			}
			if(!NavPath->PathPoints.IsEmpty())
			{
				CachedDestination = NavPath->PathPoints.Last();
			}
		}
	}
	// 此处重置bTargeting会有一些bug,例如在目标上按键时，bTargeting = true，
	// 然后移动鼠标到别处，因为没有重置bTargeting，所以会一直保持bTargeting = true
	// 直到释放鼠标。
	// 修正办法：在hold时，要检测。
	FollowTime = 0.f;
	bTargeting = false;
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// 不是左键或者在目标上释放按键时，给GameplayAbility一个处理事件的机会

	// 修正：原因见AbilityInputTagReleased
	bTargeting = ThisActor != nullptr;
	// bShiftKeyDown相当于强制添加一个bTargeting，因此适用于bTargeting的逻辑也适用于bShiftKeyDown
	const bool bForceActivate = bShiftKeyDown || bTargeting;
	if (!InputTag.MatchesTagExact(AuraGameplayTags::InputTag_LMB) || bForceActivate)
	{
		if (GetASC())
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

	if (auto ControlledPawn = GetPawn<APawn>())
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.f);
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!IsValid(AuraAbilitySystemComponent))
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AuraAbilitySystemComponent;
}