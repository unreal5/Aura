// 版权没有，拷贝自由。


#include "Character/Enemy/AuraEnemy.h"

#include "Aura/Aura.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tag/AuraGlobalTags.h"
#include "UI/Widget/AuraUserWidget.h"


// Sets default values
AAuraEnemy::AAuraEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

	// 血条组件
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed;

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	InitAbilityActorInfo();

	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);

	// 侦听Gameplay Tag
	auto TagAddedDelegate = [this](const FGameplayTag Tag, int32 NewCount)
	{
		bHitReacting = NewCount > 0;
		// 停止角色移动

		if (bHitReacting)
		{
			GetCharacterMovement()->StopMovementImmediately();
		}

		GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	};
	AbilitySystemComponent->RegisterGameplayTagEvent(Effects::HitReaction, EGameplayTagEventType::NewOrRemoved).
	                        AddLambda(TagAddedDelegate);

	// 设置UI控制器
	if (auto AuraUserWidget = Cast<UAuraUserWidget>(HealthBarWidgetComponent->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	auto BindLambda = [this](const FOnAttributeChangeData& Data, const FGameplayTag& GameplayTag)
	{
		OnAttributeValueChangedWithTag.Broadcast(GameplayTag, Data.NewValue);
	};
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetHealthAttribute()).AddLambda(
		BindLambda, Attributes::Vital::Health.GetTag());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxHealthAttribute()).
	                        AddLambda(BindLambda, Attributes::Secondary::MaxHealth.GetTag());

	// 播放初始值
	if (auto AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnAttributeValueChangedWithTag.Broadcast(Attributes::Vital::Health.GetTag(), AuraAS->GetHealth());
		OnAttributeValueChangedWithTag.Broadcast(Attributes::Secondary::MaxHealth.GetTag(), AuraAS->GetMaxHealth());
	}
}

int32 AAuraEnemy::GetPlayerLevel_Implementation() const
{
	return Level;
}

void AAuraEnemy::InitAbilityActorInfo()
{
	// Super::InitAbilityActorInfo();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeDefaultAttributes();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	// 不可以调用Super::InitializeDefaultAttributes();
	// 只在服务端初始化
	if (!HasAuthority()) return;
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}