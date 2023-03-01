// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MainMenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMainMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Asteroid Survivors MainMenuInterface
 */
class ASTEROIDSURVIVORS_API IMainMenuInterface
{
	GENERATED_BODY()

public:
	virtual void HostGame() = 0;
	virtual void JoinGame(const FString& Address) = 0;
};