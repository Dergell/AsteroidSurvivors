// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "WidgetMain.h"
#include "PlayerControllerMain.generated.h"

/**
 * Class which implements the main player controller. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API APlayerControllerMain : public APlayerController, public IItemInterface
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	// Interface called when score needs to be updated
	virtual void UpdateScore_Implementation(int32 Points) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetMain> MainWidgetClass;

private:
	// Called when the controller possesses a pawn
	virtual void OnPossess(APawn* aPawn) override;

	UPROPERTY()
	UWidgetMain* MainWidget;
};
