// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMain.generated.h"

/**
 * Class which implements the main game widget. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API UWidgetMain : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore(int32 Score);
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();
};
