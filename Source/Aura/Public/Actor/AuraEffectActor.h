// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};
class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDestroyEffectRemoval = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "应用效果 | 立即")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "应用效果 | 立即")
	EEffectApplicationPolicy InstantApplicationPolicy = EEffectApplicationPolicy::ApplyOnOverlap;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "应用效果 | 持续")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "应用效果 | 持续")
	EEffectApplicationPolicy DurationApplicationPolicy = EEffectApplicationPolicy::ApplyOnOverlap;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "应用效果 | 永久")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "应用效果 | 永久")
	EEffectApplicationPolicy InfiniteApplicationPolicy = EEffectApplicationPolicy::ApplyOnOverlap;
	// 只有永久效果才需要移除策略
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "应用效果 | 永久")
	EEffectRemovalPolicy InfiniteRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "应用效果")
	float ActorLevel = 1.f;
};
