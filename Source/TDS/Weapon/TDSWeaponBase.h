// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSItemBase.h"
#include "Types.h"
#include "TDSWeaponBase.generated.h"
	
UCLASS()
class TDS_API ATDSWeaponBase : public ATDSItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDSWeaponBase();

	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="ItemInfo")
	FOnWeaponFire OnWeaponFire;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	USphereComponent* AttackSphereComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemSettings")
	FWeaponSettings WeaponInfo;

	bool bIsMousePressed = false;
	FTimerHandle WeaponFireTimer;
	bool WeaponIsFire = false;
	
	void UpdateWeaponType(EWeaponType WType);
	
	void SpawnBullet();
	
	bool CheckWeaponCanFire();

	AActor* GetPlayerOwner() const { return GetOwner(); };
	
	UFUNCTION()
	void Fire();

	UFUNCTION()
	void StopFire();

	UFUNCTION()
	float CalculateWeaponDamage();
	
	UFUNCTION()
	void ApplyWeaponDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

