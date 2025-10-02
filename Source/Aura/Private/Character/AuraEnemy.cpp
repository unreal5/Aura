// 版权没有，拷贝自由。


#include "Character/AuraEnemy.h"

#include "Aura.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tag/GlobalTag.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// 构造ASC和AS
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AuraAbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AuraAttributeSet"));

	// 血条组件
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBar->SetDrawSize(FVector2D(100.0f, 5.0f));
	HealthBar->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));

	// 初始化角色速度
	GetCharacterMovement()->MaxWalkSpeed = BaskWalklSpeed;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	check(AbilitySystemComponent);

	InitAbilityActorInfo();

	// 给技能
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	
	auto AuraAs = Cast<UAuraAttributeSet>(AttributeSet);
	check(AuraAs);

	if (const auto AuraUserWidgetObject = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidgetObject->SetWidgetController(this);
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxHealthAttribute()).
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnMaxHealthChanged.Broadcast(Data.NewValue);
	                        });

	// 侦听Tag
	AbilitySystemComponent->RegisterGameplayTagEvent(GlobalTag::Effects_HitReact, EGameplayTagEventType::NewOrRemoved).
	                        AddLambda([this](const FGameplayTag InTag, int32 Count)
	                        {
		                        bHitReacting = Count > 0;
	                        	GetCharacterMovement()->MaxWalkSpeed = bHitReacting? 0.0f : BaskWalklSpeed;
	                        });

	OnHealthChanged.Broadcast(AuraAs->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAs->GetMaxHealth());
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	auto AuraAsc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!AuraAsc)
	{
		checkf(false,
		       TEXT("AAuraCharacter::InitAbilityActorInfo() AbilitySystemComponent is not AuraAbilitySystemComponent"));
		return;
	}
	AuraAsc->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::Die_Implementation()
{
	SetLifeSpan(LifeSpan);
	Super::Die_Implementation();
}
