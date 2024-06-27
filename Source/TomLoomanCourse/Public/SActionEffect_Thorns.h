// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:
	USActionEffect_Thorns();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Thorns")
	float ReflectedFraction;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);
};
