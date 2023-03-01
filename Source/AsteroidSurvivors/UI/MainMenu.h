// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class IMainMenuInterface;
class UButton;

/**
 * Asteroid Survivors Main Menu Widget
 */
UCLASS()
class ASTEROIDSURVIVORS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	// Used to initialize delegate callbacks
	virtual bool Initialize() override;

	// Setter
	void SetInterface(IMainMenuInterface* Interface);

	void Setup();
	void Shutdown();

private:
	UFUNCTION()
	void OnStartGameClicked();

	UFUNCTION()
	void OnQuitGameClicked();

protected:
	// Main Menu - Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;

private:
	IMainMenuInterface* MenuInterface;
};