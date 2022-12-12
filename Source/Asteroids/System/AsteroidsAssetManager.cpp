// Asteroid Survivors - Copyright (C) 2022 Tony Schmich


#include "AsteroidsAssetManager.h"
#include "AsteroidsGameplayTags.h"

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