// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "System/AsteroidsGameInstance.h"

#include "AsteroidsSaveGame.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainMenu.h"

void UAsteroidsGameInstance::Init()
{
	Super::Init();

	if (UGameplayStatics::DoesSaveGameExist(TEXT("local"), 0))
	{
		SaveGame = Cast<UAsteroidsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("local"), 0));
	}
	else
	{
		SaveGame = Cast<UAsteroidsSaveGame>(UGameplayStatics::CreateSaveGameObject(UAsteroidsSaveGame::StaticClass()));
	}

	if (GEngine)
	{
		GEngine->GameUserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		GEngine->GameUserSettings->SetScreenResolution(GEngine->GameUserSettings->GetDesktopResolution());
		GEngine->GameUserSettings->ApplySettings(true);
		GEngine->GameUserSettings->SaveSettings();
	}
}

void UAsteroidsGameInstance::Shutdown()
{
	if (SaveGame != nullptr)
	{
		UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("local"), 0);
	}

	Super::Shutdown();
}

void UAsteroidsGameInstance::HostGame()
{
	const FString LevelPath = MainGameLevel.GetLongPackageName();
	if (LevelPath.IsEmpty())
	{
		UE_LOG(LogLevel, Warning, TEXT("No MainGameLevel set in GameInstance."));
		return;
	}

	GetWorld()->ServerTravel(LevelPath);
}

void UAsteroidsGameInstance::JoinGame(const FString& Address)
{
	GetFirstLocalPlayerController()->ClientTravel(Address, TRAVEL_Absolute);
}

float UAsteroidsGameInstance::GetVolumeMusic()
{
	return VolumeMusic / 100.0;
}

float UAsteroidsGameInstance::GetVolumeEffects()
{
	return VolumeEffects / 100.0;
}

void UAsteroidsGameInstance::SaveStats(int32 Score, int32 Time) const
{
	if (!IsValid(SaveGame))
	{
		UE_LOG(LogClass, Warning, TEXT("SaveGame not valid."));
		return;
	}

	SaveGame->SetHighScore(Score);
	SaveGame->SetHighTime(Time);
}

void UAsteroidsGameInstance::SaveVolumeSettings(int32 Music, int32 Effects)
{
	if (!IsValid(SaveGame))
	{
		UE_LOG(LogClass, Warning, TEXT("SaveGame not valid."));
		return;
	}

	SaveGame->SetVolumeSettings(Music, Effects);
	VolumeMusic = Music;
	VolumeEffects = Effects;
}

void UAsteroidsGameInstance::LoadMenu()
{
	if (!IsValid(MainMenuClass))
	{
		UE_LOG(LogClass, Warning, TEXT("No MainMenuClass set in GameInstance."));
		return;
	}
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);

	if (IsValid(MainMenu))
	{
		MainMenu->Setup();
		MainMenu->SetInterface(this);

		LoadVolumeSettings();

		if (IsValid(SaveGame))
		{
			MainMenu->UpdateHighScore(SaveGame->GetHighScore());
			MainMenu->UpdateHighTime(SaveGame->GetHighTime());
		}
	}
}

void UAsteroidsGameInstance::LoadVolumeSettings()
{
	if (!IsValid(SaveGame))
	{
		UE_LOG(LogClass, Warning, TEXT("SaveGame not valid."));
		return;
	}

	int32 Music, Effects;
	SaveGame->GetVolumeSettings(Music, Effects);
	MainMenu->UpdateVolumeMusic(Music);
	MainMenu->UpdateVolumeEffects(Effects);
	VolumeMusic = Music;
	VolumeEffects = Effects;
}