// Fill out your copyright notice in the Description page of Project Settings.

#include "SHordeGameMode.h"
#include "TimerManager.h"

ASHordeGameMode::ASHordeGameMode()
{
	WaveCount = 0;
	TimeBetweenWaves = 15.f;
}

void ASHordeGameMode::StartWave()
{
	WaveCount++;

	NumberOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASHordeGameMode::SpawnBotTimerElapsed, 1.f, true, 0.f);
}


void ASHordeGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	PrepareForNextWave();
}


void ASHordeGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}

void ASHordeGameMode::PrepareForNextWave()
{
	FTimerHandle TimerHandle_NextWaveStart;
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASHordeGameMode::StartWave, TimeBetweenWaves, false);
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
