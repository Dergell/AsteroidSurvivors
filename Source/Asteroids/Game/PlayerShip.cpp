// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShip.h"
#include "Asteroids/Items/ItemProjectile.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	// Setup Components
	RootComponent = Mesh;
	SpringArm->SetupAttachment(Mesh);
	Camera->SetupAttachment(SpringArm);

	// Always simulate physics
	Mesh->SetSimulatePhysics(true);
}

void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	// Add event listeners
	Mesh->OnComponentHit.AddDynamic(this, &APlayerShip::OnHit);

	// Account for object mass
	MoveForce *= Mesh->GetMass();
}

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Custom Axis
	InputComponent->BindAxis("MoveForward", this, &APlayerShip::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerShip::MoveRight);
	InputComponent->BindAxis("TurnRight", this, &APlayerShip::TurnRight);

	// Custom Actions
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
	if (Value == 0.f) {
		if (FMath::IsNearlyZero(CurrentRotation.Roll, RollSpeed))
			NewRotation.Roll = 0;
		else
			NewRotation.Roll += FMath::IsNegative(CurrentRotation.Roll) ? RollSpeed : -RollSpeed;
	} else {
		NewRotation.Roll += Value * RollSpeed;
	}

	// Clamp roll so we don't overshoot
	NewRotation.Roll = FMath::Clamp(NewRotation.Roll, -RollLimit, RollLimit);

	// For the turn, just add the direction to the existing yaw
	NewRotation.Yaw += Value * TurnSpeed;

	// Normalize and set to Mesh
	NewRotation.Normalize();
	Mesh->SetRelativeRotation(NewRotation);
}

void APlayerShip::Shoot()
{
	// Create basic ActorSpawnParameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// Collect all arrow components tagged as "Muzzle"
	TArray<UActorComponent*> Muzzles = GetComponentsByTag(UArrowComponent::StaticClass(), FName("Muzzle"));

	for (UActorComponent* Item : Muzzles) {
		UArrowComponent* Muzzle = Cast<UArrowComponent>(Item);

		FVector MuzzleLocation = Muzzle->GetComponentLocation();
		FRotator MuzzleRotation = Muzzle->GetComponentRotation();

		AItemProjectile* Projectile = GetWorld()->SpawnActor<AItemProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Projectile)
			Projectile->ShootInDirection(MuzzleRotation.Vector());
	}
}

void APlayerShip::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float HitDirection = Hit.Normal.Z;

	if (Hit.Normal.Z > 0) {
	//	JumpCount = 0;
	}
}
