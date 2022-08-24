// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types.h"
#include "TDSAIBaseWeapon.generated.h"

UCLASS()
class TDS_API ATDSAIBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDSAIBaseWeapon();
	void StartFire();
	void StopFire();
	void Shot();
	FOnShot OnShot;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATDSProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetting")
	float MaxShotLength = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetting")
	float Damage = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetting")
	float RateOfFire = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetting")
	UParticleSystem* ShotParticle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetting")
	TArray<USoundBase*> ShotSounds;

private:
	FTimerHandle ShotTimerHandle;
	void SpawnMuzzleFX();
};