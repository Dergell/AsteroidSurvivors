// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Actors/ItemInterface.h"
#include "GameFramework/PlayerController.h"
#include "UI/PlayerWidget.h"
#include "AsteroidsPlayerController.generated.h"

/**
 * Class which implements the main player controller. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API AAsteroidsPlayerController : public APlayerController, public IItemInterface
{
	GENERATED_BODY()

public:
	AAsteroidsPlayerController();
	virtual void Tick(float DeltaSeconds) override;

	// Getter & Setter
	UFUNCTION(BlueprintCallable)
	FVector GetCrosshairPositionOnPlane() const;

	// Interfaces
	virtual void UpdateScore_Implementation(int32 Points) override;

	// Actions
	UFUNCTION(BlueprintCallable)
	void MoveCursor(FVector2D AxisValue = FVector2D::ZeroVector) const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	virtual void OnPossess(APawn* aPawn) override;

	// Actions
	void UpdateGamepad(FKey Key);
	void MoveCursorMouse() const;
	void MoveCursorGamepad(FVector2D AxisValue) const;

private:
	// Members
	UPROPERTY()
	UPlayerWidget* MainWidget;
	UPROPERTY(EditAnywhere)
	int32 CrosshairDistance = 200;
	UPROPERTY(VisibleAnywhere)
	bool GamepadActive = false;

	// Settings
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerWidget> MainWidgetClass;
};