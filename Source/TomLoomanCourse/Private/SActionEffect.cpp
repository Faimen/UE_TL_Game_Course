// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"

#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	USActionComponent* Component = GetOwningComponent();
	if (Component)
	{
		Component->RemoveAction(this);
	}
}

float USActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = UGameplayStatics::GetGameState(GetWorld());
	if (GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
