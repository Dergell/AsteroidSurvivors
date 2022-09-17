// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerControllerMain.h"

#include "PlayerShip.h"
#include "Engine/StaticMeshActor.h"

APlayerControllerMain::APlayerControllerMain()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerControllerMain::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MoveCursorMouse();
}

void APlayerControllerMain::UpdateScore_Implementation(int32 Points)
{
	MainWidget->UpdateScore(Points);
}

FVector APlayerControllerMain::GetCursorVector() const
{
	return CursorActor->GetActorLocation() - GetPawn()->GetActorLocation();
}

void APlayerControllerMain::BeginPlay()
{
	Super::BeginPlay();

	// Spawn an actor to represent the cursor/crosshair
	if (CursorClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		CursorActor = GetWorld()->SpawnActor<AStaticMeshActor>(CursorClass, FVector::ZeroVector,
			FRotator::ZeroRotator, SpawnParams);
	}
	else
	{
		UE_LOG(LogBlueprint, Warning, TEXT("Could not spawn CursorActor. Make sure to select a CursorClass."));
	}
}

void APlayerControllerMain::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputKeyBinding& AnyKey = InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this,
		&APlayerControllerMain::UpdateGamepad);
	FInputVectorAxisBinding& GamepadRight2D = InputComponent->BindVectorAxis(EKeys::Gamepad_Right2D, this,
		&APlayerControllerMain::MoveCursorGamepad);

	AnyKey.bConsumeInput = false;
	GamepadRight2D.bConsumeInput = false;
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

void APlayerControllerMain::MoveCursorMouse()
{
	if (!GamepadActive)
	{
		float MousePosX, MousePosY;
		if (GetMousePosition(MousePosX, MousePosY))
		{
			FVector TraceLocation, TraceDirection;
			DeprojectScreenPositionToWorld(MousePosX, MousePosY, TraceLocation, TraceDirection);
			const FVector MousePosPlane = FMath::LinePlaneIntersection(TraceLocation,
				TraceLocation + TraceDirection * 10000,
				FPlane(0, 0, 1, 0));

			const FVector CursorVector = MousePosPlane - GetPawn()->GetActorLocation();
			CursorActor->SetActorRotation(CursorVector.Rotation() + FRotator(0, 90, 0));
			CursorActor->SetActorLocation(MousePosPlane);
		}
	}
}

void APlayerControllerMain::MoveCursorGamepad(FVector AxisValue)
{
	if (GamepadActive)
	{
		if (AxisValue.IsNearlyZero(.20f))
		{
			CursorActor->GetStaticMeshComponent()->SetVisibility(false);
		}
		else
		{
			CursorActor->GetStaticMeshComponent()->SetVisibility(true);
			AxisValue = AxisValue.RotateAngleAxis(90, FVector::UpVector) * 1000;
		}
		const FVector CursorLocation = AxisValue.GetClampedToSize2D(300, 800) + GetPawn()->GetActorLocation();
		CursorActor->SetActorLocation(CursorLocation);
		CursorActor->SetActorRotation(AxisValue.Rotation() + FRotator(0, 90, 0));
	}
}
