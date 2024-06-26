// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


class USWorldUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMANCOURSE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float TraceDistance;
	
	UPROPERTY(EditDefaultsOnly, Category="Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category="Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	USWorldUserWidget* DefaultWidgetInstance;
	
	void FindBestInteractable();

public:
	USInteractionComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract();
};
