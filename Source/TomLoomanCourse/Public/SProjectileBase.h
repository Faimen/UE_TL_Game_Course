// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class USoundCue;
class UAudioComponent;
class UCameraShakeBase;

UCLASS(Abstract)
class TOMLOOMANCOURSE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* SphereComponent;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UParticleSystemComponent* EffectComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UAudioComponent* AudioComponent;
		
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	UParticleSystem* HitVFX;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	USoundCue* ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category="Effects|Shake")
	TSubclassOf<UCameraShakeBase> ImpactShake;

	UPROPERTY(EditDefaultsOnly, Category="Effects|Shake")
	float ImpactShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category="Effects|Shake")
	float ImpactShakeOuterRadius;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
	
	virtual void PostInitializeComponents() override;	
};
