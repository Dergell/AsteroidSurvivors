// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "System/AsteroidsGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "UI/MainMenu.h"

void UAsteroidsGameInstance::Init()
{
	Super::Init();
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
	}
}