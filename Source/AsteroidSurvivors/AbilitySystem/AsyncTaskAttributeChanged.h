﻿// Asteroid Survivors - Copyright (C) 2023 Tony Schmich

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncTaskAttributeChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);

/**
 * Blueprint node to automatically register a listener for all attribute changes in an AbilitySystemComponent.
 * Useful to use in UI.
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = AsyncTask))
class ASTEROIDSURVIVORS_API UAsyncTaskAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	// Listens for an attribute changing.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncTaskAttributeChanged* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);

	// Listens for an attribute changing.
	// Version that takes in an array of Attributes. Check the Attribute output for which Attribute changed.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncTaskAttributeChanged* ListenForAttributesChange(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
	void EndTask();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;

protected:
	void AttributeChanged(const FOnAttributeChangeData& Data);

	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayAttribute AttributeToListenFor;
	TArray<FGameplayAttribute> AttributesToListenFor;
};