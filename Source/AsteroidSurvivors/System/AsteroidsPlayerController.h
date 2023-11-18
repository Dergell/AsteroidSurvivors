// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Actors/ItemInterface.h"
#include "GameFramework/PlayerController.h"
#include "UI/PlayerWidget.h"
#include "AsteroidsPlayerController.generated.h"

class UInputConfig;

/**
 * Class which implements the main player controller. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AAsteroidsPlayerController : public APlayerController, public IItemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AAsteroidsPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Custom input bindings
	virtual void SetupInputComponent() override;

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Return the TeamId
	virtual FGenericTeamId GetGenericTeamId() const override;

	// Get the crosshair position from the MainWidget and translate it into a world position on the Z plane
	UFUNCTION(BlueprintCallable)
	FVector GetCrosshairPositionOnPlane() const;

	// Get the InputConfig
	UInputConfig* GetInputConfig() const;

	// Tells the MainWidget to update the player score
	virtual void UpdateScore_Implementation(int32 Points) override;

	// Move cursor; delegates to MoveCursorMouse or MoveCursorGamepad
	UFUNCTION(BlueprintCallable)
	void MoveCursor(FVector2D AxisValue = FVector2D::ZeroVector) const;

private:
	// Checks the last input and switches between kbm/gamepad accordingly
	void UpdateGamepad(FKey Key);

	// Move cursor to mouse position in viewport
	void MoveCursorMouse() const;

	// Move cursor around player from gamepad input
	void MoveCursorGamepad(FVector2D AxisValue) const;

	// Pause the game and show menu
	void TriggerPause();

private:
	// Main player HUD widget
	UPROPERTY()
	UPlayerWidget* MainWidget;

	// Configuration of player inputs
	UPROPERTY(EditDefaultsOnly)
	UInputConfig* InputConfig;
	
	// Distance of crosshair from player using gamepad
	UPROPERTY(EditAnywhere)
	int32 CrosshairDistance = 200;

	// Last input was from a gamepad
	UPROPERTY(VisibleAnywhere)
	bool bGamepadActive = false;

	// Class to use as MainWidget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerWidget> MainWidgetClass;

	// Used to determine affiliation
	UPROPERTY(EditAnywhere)
	FGenericTeamId TeamId = FGenericTeamId(0);
};