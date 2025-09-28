// 版权没有，拷贝自由。


#include "Character/AuraEnemy.h"

#include "Aura.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
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
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	check(AbilitySystemComponent);

	InitAbilityActorInfo();

	auto AuraUserWidgetObject = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject());
	if (AuraUserWidgetObject)
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

	auto AuraAs = Cast<UAuraAttributeSet>(AttributeSet);
	if (AuraAs)
	{
		OnHealthChanged.Broadcast(AuraAs->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAs->GetMaxHealth());
	}
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
