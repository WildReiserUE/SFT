// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "TDSUInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTDSUInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Metods
 */
class TDS_API ITDSUInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	bool AviableForEffects();

	void SpawnActivate();
};