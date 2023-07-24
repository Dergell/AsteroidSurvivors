// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "PlayerWidget.h"

void UPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const int32 TimeSeconds = GetWorld()->GetTimeSeconds();
	const int32 TimeMinutes = floor(TimeSeconds / 60);
	TextTimer->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), TimeMinutes % 60, TimeSeconds % 60)));
}