// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "MainMenu.h"

#include "MainMenuInterface.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "System/AsteroidsGameInstance.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	this->SetIsFocusable(true);
}

bool UMainMenu::Initialize()
{
	bool Result = Super::Initialize();
	if (!Result)
	{
		return Result;
	}

	if (IsValid(StartGameButton))
	{
		StartGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartGameClicked);
	}

	if (IsValid(QuitGameButton))
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuitGameClicked);
	}

	if (IsValid(SaveOptionsButton))
	{
		SaveOptionsButton->OnClicked.AddDynamic(this, &UMainMenu::OnSaveOptionsClicked);
	}

	return Result;
}

void UMainMenu::SetInterface(IMainMenuInterface* Interface)
{
	MenuInterface = Interface;
}

void UMainMenu::UpdateHighScore(int32 InHighScore)
{
	if (HighScore != nullptr)
	{
		HighScore->SetText(FText::AsNumber(InHighScore));
	}
}

void UMainMenu::UpdateHighTime(int32 InHighTime)
{
	if (HighTime != nullptr)
	{
		HighTime->SetText(FText::AsTimespan(FTimespan(0, 0, InHighTime)));
	}
}

void UMainMenu::UpdateVolumeMusic(int32 InMusic)
{
	if (IsValid(VolumeMusic))
	{
		VolumeMusic->SetValue(InMusic);
	}
}

void UMainMenu::UpdateVolumeEffects(int32 InEffects)
{
	if (IsValid(VolumeEffects))
	{
		VolumeEffects->SetValue(InEffects);
	}
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
	}
}

void UMainMenu::Shutdown()
{
	this->RemoveFromParent();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::SwitchLevel(int32 Index)
{
	if (IsValid(MenuSwitcher))
	{
		MenuSwitcher->SetActiveWidgetIndex(Index);
	}
}

void UMainMenu::OnStartGameClicked()
{
	if (ensure(MenuInterface != nullptr))
	{
		MenuInterface->HostGame();
	}
}

void UMainMenu::OnQuitGameClicked()
{
	// Seems like this is how to do it, according to UKismetSystemLibrary::QuitGame
	GetOwningLocalPlayer()->ConsoleCommand("quit");
}

void UMainMenu::OnSaveOptionsClicked()
{
	UAsteroidsGameInstance* GameInstance = GetGameInstance<UAsteroidsGameInstance>();
	if (IsValid(GameInstance))
	{
		GameInstance->SaveVolumeSettings(VolumeMusic->GetValue(), VolumeEffects->GetValue());
	}

	if (IsValid(MenuSwitcher))
	{
		MenuSwitcher->SetActiveWidgetIndex(0);
	}
}