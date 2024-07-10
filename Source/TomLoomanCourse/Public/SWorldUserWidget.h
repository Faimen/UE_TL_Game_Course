// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetAttachedActor(AActor* Actor);

protected:
	UPROPERTY(meta=(BindWidget))
	USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category="UI", meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;
	
	UPROPERTY(EditAnywhere, Category="UI")
	FVector WorldOffset;
};
