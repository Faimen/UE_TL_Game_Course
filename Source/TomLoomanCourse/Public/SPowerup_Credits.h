// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API ASPowerup_Credits : public ASPowerupActor
{
	GENERATED_BODY()

public:
	ASPowerup_Credits();

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Credits")
	int32 CreditsAmount;
};
