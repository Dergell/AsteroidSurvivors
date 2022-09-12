// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "ItemBase.h"

#include "Kismet/GameplayStatics.h"

AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

void AItemBase::Tick(float DeltaSeconds)
{
	const AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (GetDistanceTo(PlayerPawn) > KillDistance)
	{
		Destroy();
	}
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

bool AItemBase::GetIsCollectable() const
{
	return IsCollectable;
}

int32 AItemBase::GetPointsValue() const
{
	return PointsValue;
}

void AItemBase::Collected_Implementation()
{
	Destroy();
}

void AItemBase::HitByProjectile_Implementation(APawn* ProjectileInstigator, TSubclassOf<UGameplayEffect> ProjectileEffect)
{
	Destroy();
}
