// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TomLoomanCourse/TomLoomanCourse.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Starting Action: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Starting Action: %s"), *ActionName.ToString()), FColor::Green);

	USActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.AppendTags(GrantsTags);
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		TimeStarted = GetWorld()->GetTimeSeconds();
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping Action: %s"), *GetNameSafe(this));
	//LogOnScreen(this, FString::Printf(TEXT("Stopping Action: %s"), *ActionName.ToString()), FColor::Orange);

	//ensureAlways(bIsRunning);

	USActionComponent* ActionComp = GetOwningComponent();
	ActionComp->ActiveGameplayTags.RemoveTags(GrantsTags);
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool USAction::CanStart_Implementation(AActor* Instigator) const
{
	if (RepData.bIsRunning) return false;

	USActionComponent* ActionComp = GetOwningComponent();
	if (ActionComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning_Implementation() const
{
	return RepData.bIsRunning;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

UWorld* USAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, TimeStarted);
}
