// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "System/AsteroidsGameInstance.h"

#include "AsteroidsSaveGame.h"
#include "Blueprint/UserWidget.h"
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

void UAsteroidsGameInstance::UpdateSaveGame(int32 Score, int32 Time) const
{
	if (!IsValid(SaveGame))
	{
		UE_LOG(LogClass, Warning, TEXT("SaveGame not valid."));
		return;
	}

	if (Score > SaveGame->GetHighScore())
	{
		SaveGame->SetHighScore(Score);
	}

	if (Time > SaveGame->GetHighTime())
	{
		SaveGame->SetHighTime(Time);
	}
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

		if (IsValid(SaveGame))
		{
			MainMenu->UpdateHighScore(SaveGame->GetHighScore());
			MainMenu->UpdateHighTime(SaveGame->GetHighTime());
		}
	}
}