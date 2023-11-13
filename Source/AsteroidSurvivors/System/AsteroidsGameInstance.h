// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MainMenuInterface.h"
#include "AsteroidsGameInstance.generated.h"

class UAsteroidsSaveGame;
class UMainMenu;

UCLASS()
class ASTEROIDSURVIVORS_API UAsteroidsGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void HostGame() override;
	virtual void JoinGame(const FString& Address) override;

	UFUNCTION(BlueprintCallable)
	float GetVolumeMusic();
	UFUNCTION(BlueprintCallable)
	float GetVolumeEffects();

	void SaveStats(int32 Score, int32 Time) const;
	void SaveVolumeSettings(int32 Music, int32 Effects);

	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	UFUNCTION(BlueprintCallable)
	void LoadVolumeSettings();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MainGameLevel;

private:
	UPROPERTY()
	TObjectPtr<UMainMenu> MainMenu;

	UPROPERTY()
	TObjectPtr<UAsteroidsSaveGame> SaveGame;

	UPROPERTY()
	int32 VolumeMusic;
	UPROPERTY()
	int32 VolumeEffects;
};