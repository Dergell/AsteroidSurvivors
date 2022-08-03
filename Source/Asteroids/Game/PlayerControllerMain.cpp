// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerControllerMain.h"

#include "PlayerShip.h"
#include "Engine/StaticMeshActor.h"

APlayerControllerMain::APlayerControllerMain()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerControllerMain::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float MousePosX, MousePosY;
	if (GetMousePosition(MousePosX, MousePosY))
	{
		FVector TraceLocation, TraceDirection;
		DeprojectScreenPositionToWorld(MousePosX, MousePosY, TraceLocation, TraceDirection);
		const FVector MousePosPlane = FMath::LinePlaneIntersection(TraceLocation,
		                                                           TraceLocation + TraceDirection * 10000,
		                                                           FPlane(0, 0, 1, 0));

		if (APlayerShip* PlayerShip = Cast<APlayerShip>(GetPawn()))
		{
			PlayerShip->Turn(MousePosPlane, DeltaSeconds);

			const FVector CursorVector = MousePosPlane - PlayerShip->GetActorLocation();
			CursorActor->SetActorRotation(CursorVector.Rotation() + FRotator(0, 90, 0));
		}

		CursorActor->SetActorLocation(MousePosPlane);
	}
}

void APlayerControllerMain::UpdateScore_Implementation(int32 Points)
{
	MainWidget->UpdateScore(Points);
}

void APlayerControllerMain::BeginPlay()
{
	Super::BeginPlay();

	if (CursorClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		CursorActor = GetWorld()->SpawnActor<AStaticMeshActor>(CursorClass, FVector::ZeroVector,
		                                                       FRotator::ZeroRotator, SpawnParams);
	}
	else
	{
		UE_LOG(LogBlueprint, Warning, TEXT("Could not spawn CursorActor. Make sure to select a CursorClass."));
	}
}

void APlayerControllerMain::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Create the MainWidget and add to viewport
	MainWidget = CreateWidget<UWidgetMain>(this, MainWidgetClass);
	MainWidget->AddToViewport();
}
