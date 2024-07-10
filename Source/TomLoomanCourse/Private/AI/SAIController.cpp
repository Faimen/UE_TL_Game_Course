// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviourTree, TEXT("Behavior Tree is nullptr! Please assign a Behavior Tree in the Controller")))
	{
		RunBehaviorTree(BehaviourTree);
	};

	// APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if(MyPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}
}
