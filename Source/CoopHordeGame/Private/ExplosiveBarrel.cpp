// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveBarrel.h"
#include "../Public/SHealthComponent.h"
#include "../Public/ExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AExplosiveBarrel::OnHealthChanged);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;
	
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	ExplosionImpulse = 400;
}

void AExplosiveBarrel::OnHealthChanged(USHealthComponent * OwningHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Health <= 0.0f && !bExploded)
	{
		// Die
		bExploded = true;

		// Play Animation / effect
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		// Apply Upward force to Actor aka Launch
		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		MeshComp->AddImpulse(BoostIntensity, NAME_None, true);
		// Change Material
		MeshComp->SetMaterial(0, ExplosionMaterial);

		// Apply radial force
		RadialForceComp->FireImpulse();
		
		SetLifeSpan(60.0f);
	}
}
