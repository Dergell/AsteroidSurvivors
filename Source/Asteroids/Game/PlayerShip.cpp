// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerShip.h"

#include "AbilitySystemComponent.h"
#include "GameplayTask.h"
#include "PlayerStateMain.h"
#include "Asteroids/Asteroids.h"
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

FVector APlayerShip::GetTargetLocation(AActor* RequestedBy) const
{
	// Current distance to target
	float Distance = FVector::Dist(GetActorLocation(), RequestedBy->GetActorLocation());

	// First estimate, using current distance
	FVector Position = GetActorLocation() + ((GetVelocity() * 1.15) * (Distance / 5000));
	Distance = FVector::Dist(Position, RequestedBy->GetActorLocation());

	// Second estimate, using distance from first estimate
	Position = GetActorLocation() + ((GetVelocity() * 1.15) * (Distance / 5000));
	Distance = FVector::Dist(Position, RequestedBy->GetActorLocation());

	// Third estimate, using distance from second estimate
	return GetActorLocation() + ((GetVelocity() * 1.15) * (Distance / 5000));
}

UAbilitySystemComponent* APlayerShip::GetAbilitySystemComponent() const
{
	const APlayerStateMain* State = GetPlayerState<APlayerStateMain>();
	return State->GetAbilitySystemComponent();
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

	// Setup abilities
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent();
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			FString("Confirm"), FString("Cancel"),
			FString("EAbilityInputID"),
			static_cast<int32>(EAbilityInputID::Confirm),
			static_cast<int32>(EAbilityInputID::Cancel)
		);
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void APlayerShip::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerStateMain* State = GetPlayerState<APlayerStateMain>())
	{
		UAbilitySystemComponent* AbilitySystemComponent = State->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(State, this);

		State->InitializeAttributes();
		State->GiveAbilities();
	}
}

void APlayerShip::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (APlayerStateMain* State = GetPlayerState<APlayerStateMain>())
	{
		UAbilitySystemComponent* AbilitySystemComponent = State->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		State->InitializeAttributes();

		if (AbilitySystemComponent && InputComponent)
		{
			const FGameplayAbilityInputBinds Binds(
				FString("Confirm"), FString("Cancel"),
				FString("EAbilityInputID"),
				static_cast<int32>(EAbilityInputID::Confirm),
				static_cast<int32>(EAbilityInputID::Cancel)
			);
			AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
		}
	}
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
