// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponentFromActor(Pawn);
	if (ensure(AttributeComponent))
	{
		AttributeComponent->ApplyHealthChange(Pawn, AttributeComponent->GetHealthMax());
	}
	
	return EBTNodeResult::Succeeded;
}
