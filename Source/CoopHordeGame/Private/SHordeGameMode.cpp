// Fill out your copyright notice in the Description page of Project Settings.

#include "SHordeGameMode.h"
#include "../Public/SHealthComponent.h"
#include "../Public/SGameState.h"
#include "TimerManager.h"


ASHordeGameMode::ASHordeGameMode()
{
	WaveCount = 0;
	TimeBetweenWaves = 15.f;

	GameStateClass = ASGameState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;
}


void ASHordeGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if(NumberOfBotsToSpawn > 0 || bIsPreparingForWave)
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();

		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}

		USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HealthComp && HealthComp->GetHealth() > 0.f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive) 
	{
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}

}

void ASHordeGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) 
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));

			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.f)
			{
				//Player Alive
				return;
			}
		}
	}

	GameOver();

}

void ASHordeGameMode::GameOver()
{
	EndWave();

	// @TODO: Finish up the match, show feedback to user.
	SetWaveState(EWaveState::GameOver);

	UE_LOG(LogTemp, Log, TEXT("GAME OVER!"))
}

void ASHordeGameMode::SetWaveState(EWaveState WaveState)
{
	ASGameState* GS = GetGameState<ASGameState>();
	if (ensureAlways(GS))
	{
		GS->SetWaveState(WaveState);
	}
}


void ASHordeGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASHordeGameMode::SpawnBotTimerElapsed, 1.f, true, 0.f);

	SetWaveState(EWaveState::WaveInProgress);
}


void ASHordeGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	SetWaveState(EWaveState::WaitingToComplete);
}


void ASHordeGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void ASHordeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();

	CheckAnyPlayerAlive();
}

void ASHordeGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASHordeGameMode::StartWave, TimeBetweenWaves, false);
	SetWaveState(EWaveState::WaitingToStart);
}


void ASHordeGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	NumberOfBotsToSpawn--;

	if (NumberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}
