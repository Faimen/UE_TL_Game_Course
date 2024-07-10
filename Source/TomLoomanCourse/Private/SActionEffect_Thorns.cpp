// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	Duration = 0.0f;
	Period = 0.0f;

	ReflectedFraction = 0.5f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponentFromActor(
		GetOwningComponent()->GetOwner());
	if (ensure(AttributeComponent))
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponentFromActor(
		GetOwningComponent()->GetOwner());
	if (ensure(AttributeComponent))
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}

	Super::StopAction_Implementation(Instigator);
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent,
                                            float NewHealth, float Delta)
{
	//Don't reflect damage owner caused to himself and healing
	if (InstigatorActor == OwningComponent->GetOwner() || Delta >= 0.0f)
	{
		return;
	}

	//Flip the sign so that we can reflect damage with Damage function
	int32 ReflectedDamage = FMath::Abs(FMath::RoundToInt(Delta * ReflectedFraction));
	USGameplayFunctionLibrary::ApplyDamage(OwningComponent->GetOwner(), InstigatorActor, ReflectedDamage);
}
