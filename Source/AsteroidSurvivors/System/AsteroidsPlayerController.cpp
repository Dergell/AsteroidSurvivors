// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsPlayerController.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"

AAsteroidsPlayerController::AAsteroidsPlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAsteroidsPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FVector AAsteroidsPlayerController::GetCrosshairPositionOnPlane() const
{
	const UWidget* Crosshair = MainWidget->WidgetTree->FindWidget(FName("Crosshair"));
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Crosshair->Slot);

	// Translate widget local crosshair position into viewport mouse position
	FVector2D PixelPosition, ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(
		GetWorld(), MainWidget->GetCachedGeometry(),
		Slot->GetPosition(),
		PixelPosition, ViewportPosition
	);

	// Translate viewport crosshair position into 3D space position
	FVector TraceLocation, TraceDirection;
	DeprojectScreenPositionToWorld(PixelPosition.X, PixelPosition.Y, TraceLocation, TraceDirection);
	const FVector PlanePosition = FMath::LinePlaneIntersection(
		TraceLocation,
		TraceLocation + TraceDirection * 10000,
		FPlane(0, 0, 1, 0)
	);

	return PlanePosition;
}

void AAsteroidsPlayerController::UpdateScore_Implementation(int32 Points)
{
	MainWidget->UpdateScore(Points);
}

void AAsteroidsPlayerController::MoveCursor(FVector2D AxisValue) const
{
	if (!GamepadActive)
	{
		MoveCursorMouse();
	}
	else
	{
		MoveCursorGamepad(AxisValue);
	}
}

void AAsteroidsPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AAsteroidsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputKeyBinding& AnyKey = InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &AAsteroidsPlayerController::UpdateGamepad);
	AnyKey.bConsumeInput = false;
}

void AAsteroidsPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Create the MainWidget and add to viewport
	MainWidget = CreateWidget<UPlayerWidget>(this, MainWidgetClass);
	MainWidget->AddToViewport();
}

void AAsteroidsPlayerController::UpdateGamepad(FKey Key)
{
	GamepadActive = Key.IsGamepadKey() ? true : false;
}

void AAsteroidsPlayerController::MoveCursorMouse() const
{
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(MainWidget);

	const UWidget* Crosshair = MainWidget->WidgetTree->FindWidget(FName("Crosshair"));
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Crosshair->Slot);
	Slot->SetPosition(MousePosition);
}

void AAsteroidsPlayerController::MoveCursorGamepad(FVector2D AxisValue) const
{
	int32 SizeX, SizeY;
	GetViewportSize(SizeX, SizeY);
	AxisValue = AxisValue.GetSafeNormal() * CrosshairDistance + FVector2D(SizeX / 2, SizeY / 2);

	FVector2D LocalCoordinate;
	USlateBlueprintLibrary::ScreenToWidgetLocal(GetWorld(), MainWidget->GetCachedGeometry(), AxisValue, LocalCoordinate);

	const UWidget* Crosshair = MainWidget->WidgetTree->FindWidget(FName("Crosshair"));
	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Crosshair->Slot);
	Slot->SetPosition(LocalCoordinate);
}