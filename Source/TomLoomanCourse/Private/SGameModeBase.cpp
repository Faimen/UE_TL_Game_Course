// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), false, TEXT("Enable spawning of bots via timer"),
                                                ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20.0f;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 500;

	PlayerStateClass = ASPlayerState::StaticClass();
	SlotName = "Save_Test";
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	LoadSaveGame();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);

	if (ensure(PowerupClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
			this, PowerupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
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
	if (CVarSpawnBots.GetValueOnGameThread() == false)
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
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

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                                  EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Powerup Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocations;
	int32 SpawnedCounter = 0;

	while (SpawnedCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomLocationIndex];
		Locations.RemoveAt(RandomLocationIndex);

		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (OtherLocation - PickedLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				bValidLocation = false;
				break;
			}
		}

		if (!bValidLocation) continue;

		UsedLocations.Add(PickedLocation);

		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];
		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);
		SpawnedCounter++;
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
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: ActorKilled: %s, Killer: %s"), *ActorKilled->GetName(),
	       *Killer->GetName());

	ASCharacter* Player = Cast<ASCharacter>(ActorKilled);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	// Add Credits for Kill
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		if (ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}

void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // only save 1 player
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}
		FActorSaveData ActorSaveData;
		ActorSaveData.ActorName = Actor->GetName();
		ActorSaveData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ActorSaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		
		CurrentSaveGame->SavedActors.Add(ActorSaveData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);

	UE_LOG(LogTemp, Log, TEXT("Saved SaveGameObject"));
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGameObject"));
			return;
		}

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData SaveData : CurrentSaveGame->SavedActors)
			{
				if (SaveData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(SaveData.Transform);
					
					FMemoryReader MemoryReader(SaveData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGameObject"));
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created new SaveGameObject"));
	}
}
