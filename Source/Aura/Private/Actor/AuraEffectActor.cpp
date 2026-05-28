// 版权没有，拷贝自由。


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC || !InGameplayEffectClass) return false;

	auto EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	auto SpecHandle = TargetASC->MakeOutgoingSpec(InGameplayEffectClass, 1, EffectContextHandle);

	if (SpecHandle.IsValid())
	{
		auto ActivateHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		return ActivateHandle.WasSuccessfullyApplied();
	}
	return false;
}
