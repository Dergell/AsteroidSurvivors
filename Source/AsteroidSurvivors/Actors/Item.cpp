// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "Item.h"

#include "GameplayEffectTypes.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AItem::Tick(float DeltaSeconds)
{
	const AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (GetDistanceTo(PlayerPawn) > KillDistance)
	{
		Destroy();
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

bool AItem::GetIsCollectable() const
{
	return IsCollectable;
}

void AItem::MultiplyEffectAmount(float Multiplier)
{
	EffectAmount *= Multiplier;
}

void AItem::Collected_Implementation()
{
	if (IsChildActor())
	{
		GetParentActor()->Destroy();
	}
	else
	{
		Destroy();
	}
}

int32 AItem::Collect(TSubclassOf<UGameplayEffect>& OutGameplayEffect, float& OutEffectAmount)
{
	Collected();

	OutGameplayEffect = GameplayEffect;
	OutEffectAmount = EffectAmount;

	return PointsValue;
}

void AItem::HitByProjectile_Implementation(APawn* ProjectileInstigator, FGameplayEffectSpecHandle EffectSpec)
{
	Destroy();
}