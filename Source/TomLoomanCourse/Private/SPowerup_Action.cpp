// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Action.h"

#include "SAction.h"
#include "SActionComponent.h"

void ASPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(ActionToGrant && InstigatorPawn))
	{
		return;
	}

	USActionComponent* ActionComponent = InstigatorPawn->FindComponentByClass<USActionComponent>();
	if (ActionComponent)
	{
		if (ActionComponent->GetAction(ActionToGrant))
		{
			FString DebugMessage = FString::Printf(TEXT("Action %s is already granted!"), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
			return;
		}

		ActionComponent->AddAction(InstigatorPawn, ActionToGrant);
		HideAndCooldownPowerup();
	}
}
