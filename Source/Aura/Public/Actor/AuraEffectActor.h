// 版本没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlay,
	ApplyOnEndOverlay,
	DoNotApply,
};


// 只有infinite effect才会有这个选项
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlay,
	DoNotRemove,
};


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlay(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlay(AActor* TargetActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;
	// Instant
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects | Instant")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects | Instant")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Duration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects | Duration")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects | Duration")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Infinite
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects | Infinite")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects | Infinite")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects | Infinite")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlay;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel = 1.0f;

private:
	// 使用 Arthu 的方法（https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg/learn/lecture/39783288#questions/21466542）
	//TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};