// 版权没有，拷贝自由。


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 600.f;
	ProjectileMovementComponent->MaxSpeed = 600.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator);
		if (LoopingAudioComponent)
		{
			LoopingAudioComponent->Stop();
		}
	}
	Super::Destroyed();
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);

	LoopingAudioComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	FVector ImpactPoint = SweepResult.ImpactPoint;//GetActorLocation();
	// 经过以下测试，在客户端，SweepResult.ImpactPoint会是(0,0,0)，而在服务端则是正确的碰撞点。
	// 正确的行为应只在服务端注册OnComponentBeginOverlap事件。
	// if (SweepResult.ImpactPoint.IsNearlyZero())
	// {
	// 	bool bHasAuth = HasAuthority();
	// 	UE_LOG(LogTemp, Warning, TEXT("ImpactPoint is nearly zero, bHasAuth: %d"), bHasAuth);
	// }
	
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, ImpactPoint, FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, ImpactPoint, FRotator::ZeroRotator);
	if (LoopingAudioComponent)
	{
		LoopingAudioComponent->Stop();
	}
	if (HasAuthority())
	{
		if (const auto TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if (DamageEffectSpecHandle.IsValid())
			{
				TargetAsc->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
			}
		}
		Destroy();
	}
	else
	{
		bHit = true;
	}
}
