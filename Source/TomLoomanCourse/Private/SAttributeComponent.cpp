// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(GetOwner()->CanBeDamaged() == false) return false;
	
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return FMath::IsNearlyEqual(Health, HealthMax);
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);	
}

USAttributeComponent* USAttributeComponent::GetAttributeComponentFromActor(AActor* Actor)
{
	if (Actor)
	{
		return Actor->FindComponentByClass<USAttributeComponent>();
	}
	
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = GetAttributeComponentFromActor(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
	
	return false;
}
