// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "WidgetMain.h"
#include "PlayerControllerMain.generated.h"

class AStaticMeshActor;

/**
 * Class which implements the main player controller. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API APlayerControllerMain : public APlayerController, public IItemInterface
{
	GENERATED_BODY()

public:
	APlayerControllerMain();
	virtual void Tick(float DeltaSeconds) override;

	// Interface called when score needs to be updated
	virtual void UpdateScore_Implementation(int32 Points) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetMain> MainWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AStaticMeshActor> CursorClass;

private:
	// Called when the controller possesses a pawn
	virtual void OnPossess(APawn* aPawn) override;

	UPROPERTY()
	UWidgetMain* MainWidget;
	UPROPERTY()
	AStaticMeshActor* CursorActor;
};
