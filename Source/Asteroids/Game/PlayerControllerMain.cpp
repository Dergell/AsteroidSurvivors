// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerControllerMain.h"

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
