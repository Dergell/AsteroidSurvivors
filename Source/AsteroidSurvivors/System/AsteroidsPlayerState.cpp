// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsPlayerState.h"

#include "AbilitySystem/AsteroidsGameplayTags.h"
#include "AbilitySystem/AsteroidsAttributeSet.h"
#include "AbilitySystem/AsteroidsGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/PlayerShip.h"
#include "System/AsteroidsGameMode.h"

AAsteroidsPlayerState::AAsteroidsPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	Attributes = CreateDefaultSubobject<UAsteroidsAttributeSet>(TEXT("Attributes"));
}

void AAsteroidsPlayerState::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this, &AAsteroidsPlayerState::HealthChanged);

	if (bIsInvincible)
	{
		AbilitySystemComponent->AddLooseGameplayTag(FAsteroidsGameplayTags::Get().Cheat_UnlimitedHealth);
	}
}

UAbilitySystemComponent* AAsteroidsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAsteroidsPlayerState::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AAsteroidsPlayerState::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UAsteroidsGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}
	}
}

void AAsteroidsPlayerState::UpdateScore_Implementation(int32 Points)
{
	// Update the score
	PlayerScore += Points;

	// Tell the player controller to update everything that uses the current score
	IItemInterface* Interface = Cast<IItemInterface>(GetPlayerController());
	if (Interface)
	{
		Interface->Execute_UpdateScore(GetPlayerController(), PlayerScore);
	}
}

void AAsteroidsPlayerState::Die_Implementation()
{
	AbilitySystemComponent->AddLooseGameplayTag(FAsteroidsGameplayTags::Get().State_Dead);

	DisableInput(GetPlayerController());
	GetPawn<APlayerShip>()->Explode();

	const AAsteroidsGameMode* GameMode = Cast<AAsteroidsGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->GameOver();
}

void AAsteroidsPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue > Attributes->GetMaxHealth())
	{
		Attributes->SetHealth(Attributes->GetMaxHealth());
		return;
	}

	if (Data.NewValue <= 0)
	{
		Die();
	}
}