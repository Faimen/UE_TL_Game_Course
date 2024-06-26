// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("su.DebugDrawInteraction"), false, TEXT("Enable Debug Draw Interaction"), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	TraceDistance = 500.0f;
	TraceRadius = 30.0f;
	TraceChannel = ECC_WorldDynamic;
	
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebug = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams,
	                                                       Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	for (FHitResult Hit : Hits)
	{
		if (bDebug) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.f);

		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->SetAttachedActor(FocusedActor);
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebug) DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}

void USInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focused Actor to interact with");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}
