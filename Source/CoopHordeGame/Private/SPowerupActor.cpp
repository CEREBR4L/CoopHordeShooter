// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "../Public/SPowerupActor.h"


// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.f;
	TotalNoOfTicks = 0;
}

void ASPowerupActor::ActivatePowerup()
{
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerUp, PowerupInterval, true, 0.0f);
	}
	else
	{
		OnTickPowerUp();
	}
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPowerupActor::OnTickPowerUp()
{
	TicksProcessed++;

	OnPowerupTicked();

	if (TicksProcessed >= TotalNoOfTicks)
	{
		OnExpired();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}


