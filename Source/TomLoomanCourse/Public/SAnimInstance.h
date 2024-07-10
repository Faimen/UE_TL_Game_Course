// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class USActionComponent;
/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	bool IsStunned;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	USActionComponent* ActionComponent;

	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
