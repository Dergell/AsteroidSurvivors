// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerControllerMain.h"

#include "PlayerShip.h"

void APlayerControllerMain::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float MousePosX, MousePosY;
	GetMousePosition(MousePosX, MousePosY);

	FVector TraceLocation, TraceDirection;
	DeprojectScreenPositionToWorld(MousePosX, MousePosY, TraceLocation, TraceDirection);
	const FVector MousePosPlane = FMath::LinePlaneIntersection(TraceLocation,
	                                                           TraceLocation + TraceDirection * 10000,
	                                                           FPlane(0, 0, 1, 0));

	if (APlayerShip* PlayerShip = Cast<APlayerShip>(GetPawn()))
		PlayerShip->Turn(MousePosPlane, DeltaSeconds);
}

void APlayerControllerMain::UpdateScore_Implementation(int32 Points)
{
	MainWidget->UpdateScore(Points);
}

void APlayerControllerMain::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Create the MainWidget and add to viewport
	MainWidget = CreateWidget<UWidgetMain>(this, MainWidgetClass);
	MainWidget->AddToViewport();
}
