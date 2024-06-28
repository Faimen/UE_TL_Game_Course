// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionEffect.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, USActionComponent*, OwningComponent, USAction*, Action);

class USAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMANCOURSE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USActionComponent();

protected:
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<USAction*> Actions;

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerStartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopActionByName(AActor* Instigator, FName ActionName);

	bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Actions")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category="Actions")
	USAction* GetAction(TSubclassOf<USAction> ActionClass) const;
};
