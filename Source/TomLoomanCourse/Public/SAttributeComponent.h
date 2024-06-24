// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*,
                                              OwningComponent, float, NewHealth, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMANCOURSE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Attributes")
	static USAttributeComponent* GetAttributeComponentFromActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta= (DisplayName = "Is Alive"))
	static bool IsActorAlive(AActor* Actor);

	USAttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float HealthMax;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool Kill(AActor* InstigatorActor);
};
