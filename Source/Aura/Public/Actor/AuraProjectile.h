// 版权没有，拷贝自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UAudioComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();
	void BeginPlay() override;
	void Destroyed() override;


	UPROPERTY(EditAnywhere)
	float InitialSpeed = 550.f;

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 550.f;

	UPROPERTY(EditAnywhere)
	bool bHasGravity = false;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
private:
	// 抛射物是否已经击中目标，防止重复触发
	bool bHit = false;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, Transient)
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
	void PlayVFX() const;
};