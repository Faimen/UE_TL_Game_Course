// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "Powerups"

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

FText ASPowerup_HealthPotion::GetInteractMessage_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn)) return FText::GetEmpty();

	USAttributeComponent* AttributeComponent = InstigatorPawn->FindComponentByClass<USAttributeComponent>();

	if (ensure(AttributeComponent) && AttributeComponent->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return FText::Format(
		LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits\nRestore Health to Max"), CreditsCost);
}

#undef LOCTEXT_NAMESPACE
