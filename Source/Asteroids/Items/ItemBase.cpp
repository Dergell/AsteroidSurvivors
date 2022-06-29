// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemBase.h"
#include "Asteroids/Game/PlayerShip.h"
#include "Asteroids/Game/GameModeMain.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AItemBase::HitByProjectile_Implementation()
{
	Destroy();
}

void AItemBase::Collected_Implementation()
{
	AGameModeMain* GameMode = Cast<AGameModeMain>(GetWorld()->GetAuthGameMode());

	if (GameMode) {
		GameMode->ItemCollected();
	}
}

