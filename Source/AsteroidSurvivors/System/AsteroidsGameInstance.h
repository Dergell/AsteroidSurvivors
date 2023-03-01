// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MainMenuInterface.h"
#include "AsteroidsGameInstance.generated.h"

class UMainMenu;

UCLASS()
class ASTEROIDSURVIVORS_API UAsteroidsGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	virtual void HostGame() override;
	virtual void JoinGame(const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MainGameLevel;

private:
	TObjectPtr<UMainMenu> MainMenu;
};