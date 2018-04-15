// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class COOPHORDEGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

	void ActivatePowerup();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnActivated();


	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupTicked();

	UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
	bool bIsPowerupActive;

	UFUNCTION()
	void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChanged(bool bNewIsActive);

protected:

	// Time between powerup ticks
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float PowerupInterval;
	
	// Total times we apply the power up effect
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNoOfTicks;

	// Total Number of ticks applied
	int32 TicksProcessed;

	FTimerHandle TimerHandle_PowerupTick;

	UFUNCTION()
	void OnTickPowerUp();

};
