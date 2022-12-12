// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerControllerMain.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

APlayerControllerMain::APlayerControllerMain()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerControllerMain::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerControllerMain::UpdateScore_Implementation(int32 Points)
{
	MainWidget->UpdateScore(Points);
}

FVector APlayerControllerMain::GetCursorVector() const
{
	const UWidget* Crosshair = MainWidget->WidgetTree->FindWidget(FName("Crosshair"));
	const UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Crosshair->Slot);

	FVector TraceLocation, TraceDirection;
	DeprojectScreenPositionToWorld(Slot->GetPosition().X, Slot->GetPosition().Y, TraceLocation, TraceDirection);
	const FVector PlanePosition = FMath::LinePlaneIntersection(TraceLocation, TraceLocation + TraceDirection * 10000, FPlane(0, 0, 1, 0));

	return PlanePosition - GetPawn()->GetActorLocation();
}

void APlayerControllerMain::GameOver() const
{
	MainWidget->GameOver();
}

void APlayerControllerMain::MoveCursor() const
{
	if (!GamepadActive)
	{
		MoveCursorMouse();
	}
	else
	{
		//MoveCursorGamepad();
	}
}

void APlayerControllerMain::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerControllerMain::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Create the MainWidget and add to viewport
	MainWidget = CreateWidget<UWidgetMain>(this, MainWidgetClass);
	MainWidget->AddToViewport();
}

void APlayerControllerMain::UpdateGamepad(FKey Key)
{
	GamepadActive = Key.IsGamepadKey() ? true : false;
}

void APlayerControllerMain::MoveCursorMouse() const
{
	float MousePosX, MousePosY;
	if (GetMousePosition(MousePosX, MousePosY))
	{
		const UWidget* Crosshair = MainWidget->WidgetTree->FindWidget(FName("Crosshair"));
		UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Crosshair->Slot);
		Slot->SetPosition(FVector2d(MousePosX, MousePosY));
	}
}

void APlayerControllerMain::MoveCursorGamepad(FVector2d AxisValue) const
{
	if (AxisValue.IsNearlyZero(.20f))
	{
		return;
	}

	const UWidget* Crosshair = MainWidget->WidgetTree->FindWidget(FName("Crosshair"));
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Crosshair->Slot);
	Slot->SetPosition(AxisValue.ClampAxes(300, 800));
}