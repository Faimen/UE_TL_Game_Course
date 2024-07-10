// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
{
	HealthThreshold = 0.3f;
}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(Pawn))
	{
		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComponentFromActor(Pawn);
		if (ensure(AttributeComponent))
		{
			bool IsLowHealth = AttributeComponent->GetHealth() / AttributeComponent->GetHealthMax() <= HealthThreshold;

			UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
			BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, IsLowHealth);
		}
	}
}
