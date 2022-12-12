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
	APlayerControllerMain();
	virtual void Tick(float DeltaSeconds) override;

	// Getter & Setter
	UFUNCTION(BlueprintGetter)
	FVector GetCursorVector() const;

	// Interfaces
	virtual void UpdateScore_Implementation(int32 Points) override;

	// Actions
	UFUNCTION(BlueprintCallable)
	void GameOver() const;
	UFUNCTION(BlueprintCallable)
	void MoveCursor() const;

public:
	// Settings
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetMain> MainWidgetClass;

protected:
	virtual void BeginPlay() override;

	// Settings
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool GamepadActive = false;

private:
	virtual void OnPossess(APawn* aPawn) override;

	// Actions
	void UpdateGamepad(FKey Key);
	void MoveCursorMouse() const;
	void MoveCursorGamepad(FVector2d AxisValue) const;

private:
	// Members
	UPROPERTY()
	UWidgetMain* MainWidget;
};