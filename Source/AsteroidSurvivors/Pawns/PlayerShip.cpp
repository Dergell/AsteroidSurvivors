// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "PlayerShip.h"

#include "AbilitySystem/AsteroidsGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Actors/ItemInterface.h"
#include "Actors/Projectile.h"
#include "Actors/SpawnVolume.h"
#include "Camera/CameraComponent.h"
#include "Components/AsteroidsMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/AsteroidsInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/AsteroidsPlayerController.h"
#include "System/AsteroidsPlayerState.h"

APlayerShip::APlayerShip()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// Setup components
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Setup some basic settings
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;
}

void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	// Add event listeners
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamageCallback);

	// Spawn SpawnVolume
	if (SpawnVolumeClass)
	{
		const FTransform Transform = FTransform(GetActorLocation() + FVector(0, 8000, 0));
		ASpawnVolume* SpawnVolume = GetWorld()->SpawnActorDeferred<ASpawnVolume>(SpawnVolumeClass, Transform);
		if (SpawnVolume)
		{
			SpawnVolume->AttachToActor(this);
			SpawnVolume->FinishSpawning(Transform);
		}
	}
}

void APlayerShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Change camera zoom by velocity
	const float TargetLength = FMath::Lerp(BoomMinLength, BoomMaxLength, GetVelocity().Length() / MovementComponent->GetMaxSpeed());
	FMath::ExponentialSmoothingApprox(SpringArmComponent->TargetArmLength, TargetLength, DeltaSeconds, BoomSmoothingTime);

	// Change camera rotation by velocity
	FRotator CamRotation = CameraComponent->GetRelativeRotation();
	const FRotator TargetRotation = FRotator(GetVelocity().X / (MovementComponent->GetMaxSpeed() / 4), GetVelocity().Y / (MovementComponent->GetMaxSpeed() / 4), 0);
	FMath::ExponentialSmoothingApprox(CamRotation, TargetRotation, DeltaSeconds, BoomSmoothingTime);
	CameraComponent->SetRelativeRotation(CamRotation);

	const AAsteroidsPlayerController* PlayerController = Cast<AAsteroidsPlayerController>(Controller);
	MovementComponent->RotateTowardsLocation(PlayerController->GetCrosshairPositionOnPlane(), DeltaSeconds);
}

UAbilitySystemComponent* APlayerShip::GetAbilitySystemComponent() const
{
	const AAsteroidsPlayerState* State = GetPlayerState<AAsteroidsPlayerState>();
	return State->GetAbilitySystemComponent();
}

FVector APlayerShip::GetCameraLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void APlayerShip::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AAsteroidsPlayerState* State = GetPlayerState<AAsteroidsPlayerState>())
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
	check(EnhancedInputComponent);

	const AAsteroidsPlayerController* PlayerController = GetController<AAsteroidsPlayerController>();
	const UInputConfig* InputConfig = PlayerController->GetInputConfig();
	check(InputConfig);

	const FAsteroidsGameplayTags& GameplayTags = FAsteroidsGameplayTags::Get();
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Aim_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_AimMouse);
	EnhancedInputComponent->BindActionByTag(InputConfig, GameplayTags.InputTag_Aim_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_AimStick);

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
	const AAsteroidsPlayerController* PlayerController = GetController<AAsteroidsPlayerController>();
	PlayerController->MoveCursor();
}

void APlayerShip::Input_AimStick(const FInputActionValue& InputActionValue)
{
	const AAsteroidsPlayerController* PlayerController = GetController<AAsteroidsPlayerController>();
	const FVector2D AxisValue = InputActionValue.Get<FVector2D>();
	PlayerController->MoveCursor(AxisValue);
}

void APlayerShip::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItem* ItemActor = Cast<AItem>(OtherActor);
	if (ItemActor && ItemActor->GetIsCollectable())
	{
		// Collect item
		TSubclassOf<UGameplayEffect> GameplayEffect;
		float EffectValue;
		const int32 Points = ItemActor->Collect(GameplayEffect, EffectValue);

		// Update score
		const IItemInterface* Interface = Cast<IItemInterface>(GetPlayerState());
		if (Interface && Points)
		{
			Interface->Execute_UpdateScore(GetPlayerState(), Points);
		}

		// Apply gameplay effect
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		if (ASC && GameplayEffect && EffectValue)
		{
			const FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			SpecHandle.Data.Get()->SetSetByCallerMagnitude(FAsteroidsGameplayTags::Get().Effect_Value, EffectValue);

			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void APlayerShip::OnTakeAnyDamageCallback(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (CameraShakeClass && DamagedActor == this && Damage >= 0.f)
	{
		UGameplayStatics::PlayWorldCameraShake(this, CameraShakeClass, CameraComponent->GetComponentLocation(), 0, 1, 0);
	}
}