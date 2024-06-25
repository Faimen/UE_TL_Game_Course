// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "SCharacter.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> AIChar(GetWorld()); AIChar; ++AIChar)
	{
		ASAICharacter* Bot = *AIChar;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponentFromActor(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); //@todo: need to pass in instigator player
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if(CVarSpawnBots.GetValueOnGameThread() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("CVar SpawnBots is disabled"));
		return;
	}
	
	int32 SpawnedBots = 0;
	for (TActorIterator<ASAICharacter> AIChar(GetWorld()); AIChar; ++AIChar)
	{
		ASAICharacter* Bot = *AIChar;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComponentFromActor(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			SpawnedBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found alive %i Bots"), SpawnedBots);

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (SpawnedBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max Bots spawned, skipping spawn"));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* ActorKilled, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(ActorKilled);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: ActorKilled: %s, Killer: %s"), *ActorKilled->GetName(),
	       *Killer->GetName());
}
