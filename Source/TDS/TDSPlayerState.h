// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Types.h"
#include "TDSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TDS_API ATDSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void UpdateNumberKills() { ++Kills; }
	void SetTeamID(ETeamIDType TeamID) { DefaultTeamID = TeamID; }
	ETeamIDType GetTeamID() const { return DefaultTeamID; }

private:
	ETeamIDType DefaultTeamID = ETeamIDType::None;
	int32 Kills = 0;
};
