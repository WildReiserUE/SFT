/// only cpp filae change
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TDSPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "TDSCharacter.h"
#include "Engine/World.h"
#include "TDSGameMode.h"
#include "TDSPlayerState.h"


ATDSPlayerController::ATDSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ATDSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ATDSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

bool ATDSPlayerController::EnemyIsDead() const
{
	if (!GetWorld()) return false;

	const auto GameMode = Cast<ATDSGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return false;
	return GameMode->GetNumEnemyActors() == 0 ? true : false;
}

void ATDSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
}


