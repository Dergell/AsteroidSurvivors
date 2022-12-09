// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerShip.h"

#include "AbilitySystemComponent.h"
#include "GameModeMain.h"
#include "GameplayTask.h"
#include "PlayerControllerMain.h"
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
	{
		Mesh->SetPhysicsLinearVelocity(GetVelocity().GetUnsafeNormal2D() * SpeedLimit);
	}

	// Change camera zoom by velocity
	const float TargetLength = FMath::Lerp(BoomMinLength, BoomMaxLength, GetVelocity().Length() / SpeedLimit);
	FMath::ExponentialSmoothingApprox(SpringArm->TargetArmLength, TargetLength, DeltaSeconds, BoomSmoothingTime);

	// Change camera rotation by velocity
	FRotator CamRotation = Camera->GetRelativeRotation();
	FRotator TargetRotation = FRotator(GetVelocity().X / (SpeedLimit / 4), GetVelocity().Y / (SpeedLimit / 4), 0);
	FMath::ExponentialSmoothingApprox(CamRotation, TargetRotation, DeltaSeconds, BoomSmoothingTime);
	Camera->SetRelativeRotation(CamRotation);

	// Rotate pawn towards cursor
	RotatePawn();
}

UAbilitySystemComponent* APlayerShip::GetAbilitySystemComponent() const
{
	const APlayerStateMain* State = GetPlayerState<APlayerStateMain>();
	return State->GetAbilitySystemComponent();
}

void APlayerShip::HitByProjectile_Implementation(APawn* ProjectileInstigator,
	TSubclassOf<UGameplayEffect> ProjectileEffect)
{
	if (ProjectileEffect)
	{
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(ProjectileInstigator);
		const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
			ProjectileEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	if (CameraShakeClass)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeClass, Camera->GetComponentLocation(), 0, 1, 0);
	}
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
	InputComponent->BindAxis("MoveVertical", this, &APlayerShip::MoveVertical);
	InputComponent->BindAxis("MoveHorizontal", this, &APlayerShip::MoveHorizontal);

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

void APlayerShip::MoveHorizontal(float Value)
{
	if (Mesh->GetCollisionEnabled() != ECollisionEnabled::QueryAndPhysics && Mesh->GetCollisionEnabled() != ECollisionEnabled::PhysicsOnly)
	{
		return;
	}

	const FVector Horizontal = FVector::RightVector * Acceleration * Value;
	Mesh->AddForce(Horizontal, NAME_None, true);
}

void APlayerShip::MoveVertical(float Value)
{
	if (Mesh->GetCollisionEnabled() != ECollisionEnabled::QueryAndPhysics && Mesh->GetCollisionEnabled() != ECollisionEnabled::PhysicsOnly)
	{
		return;
	}

	const FVector Vertical = FVector::ForwardVector * Acceleration * Value;
	Mesh->AddForce(Vertical, NAME_None, true);
}

void APlayerShip::RotatePawn()
{
	const FRotator CurrentRotation = Mesh->GetComponentRotation();
	FRotator TargetRotation = GetController<APlayerControllerMain>()->GetCursorVector().Rotation();

	// Add some roll to the turn
	TargetRotation.Roll = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);

	// For the actual turn, just add the direction to the existing yaw
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation,
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), TurnSpeed);
	// Clamp roll so we don't overshoot
	NewRotation.Roll = FMath::Clamp(NewRotation.Roll, -RollLimit, RollLimit);
	// Cancel any pitch by physics
	NewRotation.Pitch = 0;

	// Normalize and set to Mesh
	Mesh->SetRelativeRotation(NewRotation);
}

void APlayerShip::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItemBase* ItemActor = Cast<AItemBase>(OtherActor);
	if (ItemActor && ItemActor->GetIsCollectable())
	{
		IItemInterface* Interface = Cast<IItemInterface>(GetPlayerState());
		if (Interface)
		{
			Interface->Execute_UpdateScore(GetPlayerState(), ItemActor->GetPointsValue());
		}

		ItemActor->Collected();
	}
}

void APlayerShip::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AGameModeMain* GameMode = Cast<AGameModeMain>(UGameplayStatics::GetGameMode(GetWorld()));
	if (NormalImpulse.Size() < GameMode->GetCollisionThreshold())
	{
		return;
	}

	if (CameraShakeClass)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeClass, Camera->GetComponentLocation(), 0, 1, 0);
	}

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(OtherActor);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		GameMode->GetCollisionGameplayEffectClass(), 1, EffectContext);
	if (SpecHandle.IsValid())
	{
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
