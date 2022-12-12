// Asteroid Survivors - Copyright (C) 2022 Tony Schmich

#include "PlayerStateMain.h"

#include "AbilitySystemComponent.h"
#include "AsteroidsGameplayTags.h"
#include "Asteroids/Gameplay/AttributeSetBase.h"
#include "Asteroids/Gameplay/GameplayAbilityBase.h"

APlayerStateMain::APlayerStateMain()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	Attributes = CreateDefaultSubobject<UAttributeSetBase>(TEXT("Attributes"));
}

UAbilitySystemComponent* APlayerStateMain::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APlayerStateMain::UpdateScore_Implementation(int32 Points)
{
	// Update the score
	Score += Points;

	// Tell the player controller to update everything that uses the current score
	IItemInterface* Interface = Cast<IItemInterface>(GetPlayerController());
	if (Interface)
	{
		Interface->Execute_UpdateScore(GetPlayerController(), Score);
	}
}

void APlayerStateMain::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void APlayerStateMain::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbilityBase>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, INDEX_NONE, this));
		}
	}
}

void APlayerStateMain::BeginPlay()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this,
		&APlayerStateMain::HealthChanged);

	Super::BeginPlay();
}

void APlayerStateMain::Die_Implementation()
{
	AbilitySystemComponent->AddLooseGameplayTag(FAsteroidsGameplayTags::Get().State_Dead);

	DisableInput(GetPlayerController());
}

void APlayerStateMain::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0)
	{
		Die();
	}
}