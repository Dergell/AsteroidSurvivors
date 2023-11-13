// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class USlider;
class UWidgetSwitcher;
class IMainMenuInterface;
class UButton;
class UTextBlock;

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
	void UpdateHighScore(int32 InHighScore);
	void UpdateHighTime(int32 InHighTime);
	void UpdateVolumeMusic(int32 InMusic);
	void UpdateVolumeEffects(int32 InEffects);

	void Setup();
	void Shutdown();

protected:
	UFUNCTION(BlueprintCallable)
	void SwitchLevel(int32 Index);

private:
	UFUNCTION()
	void OnStartGameClicked();
	UFUNCTION()
	void OnQuitGameClicked();
	UFUNCTION()
	void OnSaveOptionsClicked();

protected:
	// Menu Switcher
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuSwitcher;

	// Main Menu - Buttons
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SaveOptionsButton;

	// Main Menu - Sliders
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> VolumeMusic;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> VolumeEffects;

	// Main Menu - TextBlocks
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> HighScore;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> HighTime;

private:
	IMainMenuInterface* MenuInterface;
};