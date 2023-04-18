// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportProjectile.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	ASTeleportProjectile();
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category="Teleport")
	float ExplodeDelay;

	FTimerHandle TimerHandle_ExplodeDelay;

	void Teleport();
		
	// Base class using BlueprintNativeEvent, we must override the _Implementation not the Explode()
	virtual void Explode_Implementation() override;

	virtual void BeginPlay() override;
};
