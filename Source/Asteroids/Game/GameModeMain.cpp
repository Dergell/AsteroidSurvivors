// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeMain.h"
#include "WidgetMain.h"
#include "Asteroids/Items/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void AGameModeMain::BeginPlay()
{
	// Collect all Items of class ItemBase
	TArray<AActor*> Items;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemBase::StaticClass(), Items);

	// Make sure we have a GameWidgetClass
	if (GameWidgetClass) {
		GameWidget = Cast<UWidgetMain>(CreateWidget(GetWorld(), GameWidgetClass));

		// Make sure we have a GameWidget
		if (GameWidget) {
			// Add it to the Viewport and update it
			GameWidget->AddToViewport();
			UpdateItemText();
		}
	}
}

void AGameModeMain::UpdateItemText()
{
	// Update current points in GameWidget
	GameWidget->UpdatePoints(Points);
}

void AGameModeMain::ItemCollected()
{
	// Increase Points and update the GameWidget
	Points += 500;
	UpdateItemText();
}
