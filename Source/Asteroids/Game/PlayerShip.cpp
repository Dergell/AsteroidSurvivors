// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerShip.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "Asteroids/Items/ItemProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"

APlayerShip::APlayerShip()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// Setup components
	RootComponent = Mesh;
	SpringArm->SetupAttachment(Mesh);
	Camera->SetupAttachment(SpringArm);

	// Always simulate physics since we're floating
	Mesh->SetSimulatePhysics(true);
}

void APlayerShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float TargetOffet = FMath::Pow(GetVelocity().Length() * BoomExtensionSpeed, BoomExtensionPower);
	float TargetValue = FMath::Min(BoomMaxLength, BoomMinLength + TargetOffet);
	FMath::ExponentialSmoothingApprox(SpringArm->TargetArmLength, TargetValue, DeltaSeconds, BoomSmoothingTime);
}

void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	// Add event listeners
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerShip::OnBeginOverlap);
	Mesh->OnComponentHit.AddDynamic(this, &APlayerShip::OnHit);

	// Account for object mass
	MoveForce *= Mesh->GetMass();
}

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup axis
	InputComponent->BindAxis("MoveForward", this, &APlayerShip::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerShip::MoveRight);
	InputComponent->BindAxis("TurnRight", this, &APlayerShip::TurnRight);

	// Setup actions
	InputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerShip::Shoot);
}

void APlayerShip::MoveRight(float Value)
{
	const FVector Right = Mesh->GetRightVector() * MoveForce * Value;
	Mesh->AddForce(Right);
}

void APlayerShip::MoveForward(float Value)
{
	const FVector Forward = Mesh->GetForwardVector() * MoveForce * Value;
	Mesh->AddForce(Forward);
}

void APlayerShip::TurnRight(float Value)
{
	const FRotator CurrentRotation = Mesh->GetComponentRotation();
	FRotator NewRotation = CurrentRotation;

	// Slightly roll in turn direction and roll back when the turn stops
	if (Value == 0.f)
	{
		if (FMath::IsNearlyZero(CurrentRotation.Roll, RollSpeed))
			NewRotation.Roll = 0;
		else
			NewRotation.Roll += FMath::IsNegative(CurrentRotation.Roll) ? RollSpeed : -RollSpeed;
	}
	else
	{
		NewRotation.Roll += Value * RollSpeed;
	}

	// Cancel any pitch
	NewRotation.Pitch = 0;
	// Clamp roll so we don't overshoot
	NewRotation.Roll = FMath::Clamp(NewRotation.Roll, -RollLimit, RollLimit);

	// For the actual turn, just add the direction to the existing yaw
	NewRotation.Yaw += Value * TurnSpeed;

	// Normalize and set to Mesh
	NewRotation.Normalize();
	Mesh->SetRelativeRotation(NewRotation);
}

void APlayerShip::Shoot()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	TArray<UActorComponent*> Muzzles = GetComponentsByTag(UArrowComponent::StaticClass(), FName("Muzzle"));
	for (UActorComponent* Item : Muzzles)
	{
		UArrowComponent* Muzzle = Cast<UArrowComponent>(Item);
		FVector MuzzleLocation = Muzzle->GetComponentLocation();
		FRotator MuzzleRotation = Muzzle->GetComponentRotation();

		AItemProjectile* Projectile = GetWorld()->SpawnActor<AItemProjectile>(
			ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
			Projectile->ShootInDirection(MuzzleRotation.Vector());
	}
}

void APlayerShip::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	AItemBase* ItemActor = Cast<AItemBase>(OtherActor);
	if (ItemActor && ItemActor->GetIsCollectable())
	{
		IItemInterface* Interface = Cast<IItemInterface>(GetPlayerState());
		if (Interface)
			Interface->Execute_UpdateScore(GetPlayerState(), ItemActor->GetPointsValue());

		ItemActor->Collected();
	}
}

void APlayerShip::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	// TODO
}
