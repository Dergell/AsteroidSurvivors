// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsPlayerController.h"

#include "AsteroidsGameInstance.h"
#include "AbilitySystem/AsteroidsGameplayTags.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetComponent.h"
#include "Input/AsteroidsInputComponent.h"
#include "Input/InputConfig.h"
#include "Kismet/GameplayStatics.h"

AAsteroidsPlayerController::AAsteroidsPlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAsteroidsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Create the MainWidget and add to viewport
	MainWidget = CreateWidget<UPlayerWidget>(this, MainWidgetClass);
	MainWidget->AddToViewport();
}

void AAsteroidsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputKeyBinding& AnyKey = InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &AAsteroidsPlayerController::UpdateGamepad);
	AnyKey.bConsumeInput = false;

	UAsteroidsInputComponent* EnhancedInputComponent = Cast<UAsteroidsInputComponent>(InputComponent);
	if (EnhancedInputComponent && InputConfig)
	{
		const FAsteroidsGameplayTags& GameplayTags = FAsteroidsGameplayTags::Get();
		EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Pause, ETriggerEvent::Triggered, this, &ThisClass::TriggerPause);
	}
}

void AAsteroidsPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

FGenericTeamId AAsteroidsPlayerController::GetGenericTeamId() const
{
	return TeamId;
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

UInputConfig* AAsteroidsPlayerController::GetInputConfig() const
{
	return InputConfig;
}

void AAsteroidsPlayerController::UpdateScore_Implementation(int32 Points)
{
	MainWidget->UpdateScore(Points);
}

void AAsteroidsPlayerController::MoveCursor(FVector2D AxisValue) const
{
	if (!bGamepadActive)
	{
		MoveCursorMouse();
	}
	else
	{
		MoveCursorGamepad(AxisValue);
	}
}

void AAsteroidsPlayerController::UpdateGamepad(FKey Key)
{
	if (bGamepadActive == Key.IsGamepadKey())
	{
		return;
	}
	
	bGamepadActive = Key.IsGamepadKey() ? true : false;

	TArray<AActor*> WidgetActors;
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("TutorialWidget"), WidgetActors);
	for (const AActor* WidgetActor : WidgetActors)
	{
		TArray<UWidgetComponent*> WidgetComponents;
		WidgetActor->GetComponents<UWidgetComponent>(WidgetComponents);
		for (UWidgetComponent* WidgetComponent : WidgetComponents)
		{
			WidgetComponent->ToggleVisibility();
		}
	}
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

void AAsteroidsPlayerController::TriggerPause()
{
	Pause();

	UAsteroidsGameInstance* Instance = GetGameInstance<UAsteroidsGameInstance>();
	if (Instance == nullptr)
	{
		return;
	}

	Instance->LoadMenu();
}