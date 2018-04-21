// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SHordeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COOPHORDEGAME_API ASHordeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected: 

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();
	
	void SpawnBotTimerElapsed();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	FTimerHandle TimerHandle_BotSpawner;

	int32 NumberOfBotsToSpawn;

	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	float TimeBetweenWaves;

public:

	ASHordeGameMode();

	virtual void StartPlay() override;
};
