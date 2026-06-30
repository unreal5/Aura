// 版权没有，拷贝自由。


#include "Player/AuraPlayerController.h"

#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
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
#include "UI/Widget/DamageTextComponent.h"

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

	AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		if (auto DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass))
		{
			DamageTextComponent->RegisterComponent();
			DamageTextComponent->SetDamageText(Damage);
			// 调整位置
			DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(),
			                                       FAttachmentTransformRules::KeepRelativeTransform);
			// 位置已经调整到角色上方
			DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			
		}
	}
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
	// Bind Shift Input Action
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftKeyPressed, true);
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftKeyPressed, false);

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

void AAuraPlayerController::ShiftKeyPressed(bool bKeyPressed)
{
	bShiftPressed = bKeyPressed;
}

void AAuraPlayerController::TryHighlightActor(AActor* Actor)
{
	if (IsValid(Actor) &&
		Actor->GetClass()->ImplementsInterface(UHighlightInterface::StaticClass()))
	{
		IHighlightInterface::Execute_HighlightActor(Actor);
	}
}

void AAuraPlayerController::TryUnHighlightActor(AActor* Actor)
{
	if (IsValid(Actor) &&
		Actor->GetClass()->ImplementsInterface(UHighlightInterface::StaticClass()))
	{
		IHighlightInterface::Execute_UnHighlightActor(Actor);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

	CurrentActor = CursorHit.bBlockingHit ? CursorHit.GetActor() : nullptr;
	if (LastActor != CurrentActor)
	{
		TryUnHighlightActor(LastActor);
		TryHighlightActor(CurrentActor);
	}
	LastActor = CurrentActor;
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

	// 无论如何都向ASC报告输入释放事件，这样技能系统才能正确处理输入状态。
	ASC->AbilityInputTagReleased(InputTag);
	if (!bShiftPressed && !bTargeting && InputTag.MatchesTagExact(InputAction::LMB))
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		APawn* ControlledPawn = GetPawn();
		if ((FollowTime < ShortPressThreshold) && IsValid(ControlledPawn))
		{
			FVector StartLocation = ControlledPawn->GetActorLocation();

			UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, StartLocation, CachedDestination);
			if (NavPath)
			{
				SplineComponent->ClearSplinePoints();
				for (const FVector& Point : NavPath->PathPoints)
				{
					SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(), Point, 10.0f, 12, FColor::Green, false, 5.0f);
				}
				bAutoRunning = true;
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints.Last();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("没有找到有效的导航路径"));
				}
			}

			// 产生点击地面特效
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator,
			                                               FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None,
			                                               true);
		}
	}
}

void AAuraPlayerController::AbilityInputHeld(const FGameplayTag InputTag)
{
	// 如果正在锁定目标，或者不是左键持续按下，都直接传递输入给ASC，不进行自动移动。
	if (bShiftPressed || bTargeting || !InputTag.MatchesTagExact(InputAction::LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}