// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerWidget.generated.h"

/**
 * Class which implements the main game widget. Extended by blueprints.
 */
UCLASS()
class ASTEROIDSURVIVORS_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Actions
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore(int32 Score);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextTimer;
};