// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#include "AsteroidsAssetManager.h"
#include "AbilitySystem/AsteroidsGameplayTags.h"

UAsteroidsAssetManager::UAsteroidsAssetManager()
{
}

UAsteroidsAssetManager& UAsteroidsAssetManager::Get()
{
	check(GEngine);

	UAsteroidsAssetManager* MyAssetManager = Cast<UAsteroidsAssetManager>(GEngine->AssetManager);
	return *MyAssetManager;
}

void UAsteroidsAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Load Native Tags
	FAsteroidsGameplayTags::InitializeNativeTags();
}