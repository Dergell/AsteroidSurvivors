// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerShip.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "Asteroids/Items/ItemProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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

	// Clamp velocity
	if (GetVelocity().Length() > SpeedLimit)
		Mesh->SetPhysicsLinearVelocity(GetVelocity().GetUnsafeNormal2D() * SpeedLimit);

	// Change camera zoom by velocity
	const float TargetLength = FMath::Lerp(BoomMinLength, BoomMaxLength, GetVelocity().Length() / SpeedLimit);
	FMath::ExponentialSmoothingApprox(SpringArm->TargetArmLength, TargetLength, DeltaSeconds, BoomSmoothingTime);

	// Change camera rotation by velocity
	FRotator CamRotation = Camera->GetRelativeRotation();
	FRotator TargetRotation = FRotator(GetVelocity().X / (SpeedLimit / 4), GetVelocity().Y / (SpeedLimit / 4), 0);
	FMath::ExponentialSmoothingApprox(CamRotation, TargetRotation, DeltaSeconds, BoomSmoothingTime);
	Camera->SetRelativeRotation(CamRotation);
}

void APlayerShip::Turn(FVector TargetLocation, float DeltaTime)
{
	const FRotator CurrentRotation = Mesh->GetComponentRotation();
	const FVector TargetVector = TargetLocation - Mesh->GetComponentLocation();
	FRotator TargetRotation = TargetVector.Rotation();

	// Add some roll to the turn
	TargetRotation.Roll = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);

	// For the actual turn, just add the direction to the existing yaw
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnSpeed);
	// Clamp roll so we don't overshoot
	NewRotation.Roll = FMath::Clamp(NewRotation.Roll, -RollLimit, RollLimit);
	// Cancel any pitch by physics
	NewRotation.Pitch = 0;

	// Normalize and set to Mesh
	Mesh->SetRelativeRotation(NewRotation);
}

void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	// Add event listeners
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerShip::OnBeginOverlap);
	Mesh->OnComponentHit.AddDynamic(this, &APlayerShip::OnHit);
}

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup axis
	InputComponent->BindAxis("MoveForward", this, &APlayerShip::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerShip::MoveRight);

	// Setup actions
	InputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerShip::Shoot);
}

void APlayerShip::MoveRight(float Value)
{
	const FVector Right = Mesh->GetRightVector() * Acceleration * Value;
	Mesh->AddForce(Right, NAME_None, true);
}

void APlayerShip::MoveForward(float Value)
{
	const FVector Forward = Mesh->GetForwardVector() * Acceleration * Value;
	Mesh->AddForce(Forward, NAME_None, true);
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

		GetWorld()->SpawnActor<AItemProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
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
	if (CameraShakeClass)
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeClass, Camera->GetComponentLocation(), 0, 1, 0);
}
