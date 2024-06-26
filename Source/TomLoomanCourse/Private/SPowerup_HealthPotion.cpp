// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	CreditsCost = 30;
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn)) return;

	USAttributeComponent* AttributeComponent = InstigatorPawn->FindComponentByClass<USAttributeComponent>();

	if (ensure(AttributeComponent) && !AttributeComponent->IsFullHealth())
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditsCost) && AttributeComponent->ApplyHealthChange(
				this, AttributeComponent->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}
