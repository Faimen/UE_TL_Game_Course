// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class USAttributeComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> PrimaryAbilityClass;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> SecondaryAbilityClass;
	
	FTimerHandle TimerHandle_PrimaryAttack;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USAttributeComponent* AttributeComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void PrimaryInteract();

	void PrimaryAttack();
	void PrimaryAttack_Timelapsed();
	
	void PrimaryAbility();
	void PrimaryAbility_Timelapsed();

	void SecondaryAbility();
	void SecondaryAbility_Timelapsed();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);

	FVector GetViewPosition();
	FVector GetPawnViewLocation() const override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
