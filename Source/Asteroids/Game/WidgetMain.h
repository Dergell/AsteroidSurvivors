// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMain.generated.h"

/**
 * Class which implements the main game widget. Extended by blueprints.
 */
UCLASS()
class ASTEROIDS_API UWidgetMain : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** Update the displayed points */
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePoints(int32 Points);
};
