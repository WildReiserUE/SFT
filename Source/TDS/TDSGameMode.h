// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types.h"
#include "TDSGameMode.generated.h"

UCLASS(minimalapi)
class ATDSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATDSGameMode();
	int32 GetNumEnemyActors() const;
	void SetNumEnemyActors(ETeamIDType CurrentType);
	void OneActorIsDead();
	int32 SetNumEnemyActors();

private:
	int32 MaxControllers = 0;
	int32 AlliesIndex = 0;
	int32 MaxEnemiesNum = 0;
	int32 TotalTeam_1 = 0;
	int32 TotalTeam_2 = 0;
};



