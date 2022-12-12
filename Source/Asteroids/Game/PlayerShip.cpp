// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerShip.h"

#include "AbilitySystemComponent.h"
#include "AsteroidsGameplayTags.h"
#include "GameModeMain.h"
#include "PlayerControllerMain.h"
#include "PlayerStateMain.h"
#include "Asteroids/Interfaces/ItemInterface.h"
#include "Asteroids/Items/ItemProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/AsteroidsInputComponent.h"
#include "Kismet/GameplayStatics.h"

APlayerShip::APlayerShip()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

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

	// Change camera zoom by velocity
	const float TargetLength = FMath::Lerp(BoomMinLength, BoomMaxLength, MovementComponent->Velocity.Length() / MovementComponent->MaxSpeed);
	FMath::ExponentialSmoothingApprox(SpringArm->TargetArmLength, TargetLength, DeltaSeconds, BoomSmoothingTime);

	// Change camera rotation by velocity
	FRotator CamRotation = Camera->GetRelativeRotation();
	FRotator TargetRotation = FRotator(MovementComponent->Velocity.X / (MovementComponent->MaxSpeed / 4), MovementComponent->Velocity.Y / (MovementComponent->MaxSpeed / 4), 0);
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

void APlayerShip::Explode_Implementation()
{
	Mesh->SetVisibility(false, true);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MovementComponent->Deactivate();
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

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UAsteroidsInputComponent* EnhancedInputComponent = Cast<UAsteroidsInputComponent>(InputComponent);

	//Make sure to set your input component class in the InputSettings->DefaultClasses
	check(EnhancedInputComponent);

	const FAsteroidsGameplayTags& GameplayTags = FAsteroidsGameplayTags::Get();

	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &APlayerShip::Input_Move);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Aim_Mouse, ETriggerEvent::Triggered, this, &APlayerShip::Input_AimMouse);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Aim_Stick, ETriggerEvent::Triggered, this, &APlayerShip::Input_AimStick);

	TArray<uint32> BindHandles;
	EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
}

void APlayerShip::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		TArray<FGameplayAbilitySpec*> Abilities;
		ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(InputTag), Abilities);
		for (FGameplayAbilitySpec* Ability : Abilities)
		{
			ASC->AbilityLocalInputPressed(Ability->InputID);
		}
	}
}

void APlayerShip::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		TArray<FGameplayAbilitySpec*> Abilities;
		ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(FGameplayTagContainer(InputTag), Abilities);
		for (FGameplayAbilitySpec* Ability : Abilities)
		{
			ASC->AbilityLocalInputReleased(Ability->InputID);
		}
	}
}

void APlayerShip::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MoveValue = InputActionValue.Get<FVector2D>();

	if (MoveValue.X != 0.0f)
	{
		AddMovementInput(FVector::RightVector, MoveValue.X);
	}

	if (MoveValue.Y != 0.0f)
	{
		AddMovementInput(FVector::ForwardVector, MoveValue.Y);
	}
}

void APlayerShip::Input_AimMouse(const FInputActionValue& InputActionValue)
{
	const APlayerControllerMain* PlayerController = GetController<APlayerControllerMain>();
	PlayerController->MoveCursor();
}

void APlayerShip::Input_AimStick(const FInputActionValue& InputActionValue)
{
	const APlayerControllerMain* PlayerController = GetController<APlayerControllerMain>();
	const FVector2D AimValue = InputActionValue.Get<FVector2D>();
	const FRotator CurrentRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	PlayerController->MoveCursor();
}

void APlayerShip::RotatePawn()
{
	const FRotator CurrentRotation = GetControlRotation();
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
	Controller->SetControlRotation(NewRotation);
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