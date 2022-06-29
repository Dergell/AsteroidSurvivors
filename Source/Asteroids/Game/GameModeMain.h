// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeMain.generated.h"

/**
 * Class which implements the main game mode. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API AGameModeMain : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	/** Stores the current points */
	int32 Points = 0;

	/** Property which stores the class of widget to use */
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<class UWidgetMain> GameWidgetClass;

	/** The GameWidget itself */
	UPROPERTY()
	class UWidgetMain* GameWidget;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Update the item text */
	void UpdateItemText();

public:

	/** Will be called when an item has been collected */
	void ItemCollected();
};
