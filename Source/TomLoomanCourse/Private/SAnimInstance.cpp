// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"

#include "GameplayTagContainer.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* Actor = GetOwningActor();
	if (Actor)
	{
		ActionComponent = Actor->FindComponentByClass<USActionComponent>();
	}
	IsStunned = false;
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComponent)
	{
		IsStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
