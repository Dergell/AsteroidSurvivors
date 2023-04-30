// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "MainMenu.h"

#include "MainMenuInterface.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	this->bIsFocusable = true;
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

	return Result;
}

void UMainMenu::SetInterface(IMainMenuInterface* Interface)
{
	MenuInterface = Interface;
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