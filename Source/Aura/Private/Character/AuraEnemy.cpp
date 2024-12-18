// 版本没有，拷贝自由。


#include "Character/AuraEnemy.h"

#include "Aura.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// 构造ASC和AS
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AuraAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;


	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AuraAttributeSet"));

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());

	// 让触角自动持有控制器
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	// AI 只在服务器端执行；客户端只是其复制品。
	AuraAIController = Cast<AAuraAIController>(NewController);
	// TODO: 以下代码在AuraAIController中运行，更合适。
	// 初始化黑板和行为树
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTreeAsset);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReactiong"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),
	                                                           CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::HighlightActor()
{
	// 基类已经创建Mesh和Weapon，不需要进一步检测空指针
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	if(AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	Super::Die();
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitAbilityActorInfo();
	// 只在服务器端初始化技能
	if (HasAuthority())
	{
		// 初始化技能
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}


	// 侦听"Hit react" Tag的变化
	auto&& HitReactDelegate =
		AbilitySystemComponent->RegisterGameplayTagEvent(AuraGameplayTags::Effects_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved);
	HitReactDelegate.AddLambda([this](const FGameplayTag GameplayTag, int32 TagCount)
	{
		bHitReacting = TagCount > 0;
		GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.0f : BaseWalkSpeed;
		// 只在服务端运行，因此要检查控制器是否存在
		if (AuraAIController)
		{
			AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
		}
	});

	//set widget controller
	if (auto AuraUserWidget = Cast<UAuraUserWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	// bind delegates
	auto AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	auto&& HealthDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UAuraAttributeSet::GetHealthAttribute());
	HealthDelegate.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	auto&& MaxHealthDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UAuraAttributeSet::GetMaxHealthAttribute());
	MaxHealthDelegate.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	// broadcast initial health & MaxHealth

	if (AuraAttributeSet)
	{
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	// Super::InitAbilityActorInfo();
	// 初始化ASC
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->AbilityActorInfoSet();
	}

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AAuraEnemy::InitializeDefaultAttributes()
{
	//Super::InitializeDefaultAttributes();
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}