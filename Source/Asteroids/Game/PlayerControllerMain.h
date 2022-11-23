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

	UFUNCTION(BlueprintGetter)
	FVector GetCursorVector() const;

	UFUNCTION(BlueprintCallable)
	void SetCursorHidden(bool NewValue) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWidgetMain> MainWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AStaticMeshActor> CursorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool GamepadActive = false;

private:
	// Called when the controller possesses a pawn
	virtual void OnPossess(APawn* aPawn) override;

	void UpdateGamepad(FKey Key);
	void MoveCursorMouse();
	void MoveCursorGamepad(FVector AxisValue);

	UPROPERTY()
	UWidgetMain* MainWidget;
	UPROPERTY()
	AStaticMeshActor* CursorActor;
};
