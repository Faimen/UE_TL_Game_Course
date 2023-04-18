// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ASTeleportProjectile::ASTeleportProjectile() : Super()
{
	TeleportDelay = 0.2f;
	ExplodeDelay = 0.2f;

	MovementComponent->InitialSpeed = 6000.f;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_ExplodeDelay, this, &ASTeleportProjectile::Explode, ExplodeDelay);
}

void ASTeleportProjectile::Teleport()
{
	APawn* MyInstigator = this->GetInstigator();

	if (ensure(MyInstigator))
	{
		MyInstigator->TeleportTo(GetActorLocation(), MyInstigator->GetActorRotation());
	}

	Destroy();
}

void ASTeleportProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ExplodeDelay);
	EffectComponent->DeactivateSystem();
	MovementComponent->StopMovementImmediately();

	SetActorEnableCollision(false);

	UGameplayStatics::SpawnEmitterAtLocation(this, HitVFX, GetActorLocation(), GetActorRotation());

	FTimerHandle TimerHandle_Teleport;
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASTeleportProjectile::Teleport, TeleportDelay);
}
