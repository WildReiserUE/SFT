// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSAIBaseCharacter.h"
#include "TDSDistantAttackCharacter.generated.h"

class ATDSAIBaseWeapon;

UCLASS()
class TDS_API ATDSDistantAttackCharacter : public ATDSAIBaseCharacter
{
	GENERATED_BODY()
	
public:
	void StartFire();
	void StopFire();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATDSAIBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShotAnimMontage = nullptr;

	virtual void OnDead() override;

private:
	ATDSAIBaseWeapon* CurrentWeapon = nullptr;

	void OnShot();
	void SpawnWeapon();
};
