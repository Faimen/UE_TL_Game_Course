// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class URadialForceComponent;
class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	ASExplosiveBarrel();

protected:
	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForce;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	virtual void Tick(float DeltaTime) override;
};
