// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class USActionComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class TOMLOOMANCOURSE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer GrantsTags;
	UPROPERTY(EditDefaultsOnly, Category="Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category="Action")
	USActionComponent* GetOwningComponent() const;

	bool bIsRunning;
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	bool CanStart(AActor* Instigator) const;

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	bool IsRunning() const;
	
	UWorld* GetWorld() const override;
};