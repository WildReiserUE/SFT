// Copyright Epic Games, Inc. All Rights Reserved.

#include "TDSGameMode.h"
#include "TDSPlayerController.h"
#include "TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"

ATDSGameMode::ATDSGameMode()
{
	PlayerControllerClass = ATDSPlayerController::StaticClass();
}

void ATDSGameMode::SetNumEnemyActors(ETeamIDType CurrentType)
{
	if (CurrentType == ETeamIDType::Team_2)
	{
		++TotalTeam_2;
		MaxEnemiesNum = TotalTeam_2;
		UE_LOG(LogTemp, Error, TEXT("Enemies = %i"), MaxEnemiesNum);
	}
}

void ATDSGameMode::OneActorIsDead()
{
	TotalTeam_2 = FMath::Clamp(TotalTeam_2 - 1, 0, MaxEnemiesNum);

	UE_LOG(LogTemp, Error,TEXT("EnemiesIndex = %i"), TotalTeam_2);
}

int32 ATDSGameMode::GetNumEnemyActors() const
{
	return TotalTeam_2;
}