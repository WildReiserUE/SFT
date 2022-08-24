// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/TDSItemBase.h"
#include "TDSDiary.generated.h"

UCLASS()
class TDS_API ATDSDiary : public ATDSItemBase
{
	GENERATED_BODY()
	
public:
	ATDSDiary();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DiaryInfo")
	FDiarySettings DiaryInfo;	
	
};
