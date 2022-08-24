// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSDiary.h"
#include "TDSWeaponBase.h"
#include "TDSItemBase.h"
#include "Types.h"
#include "Components/ActorComponent.h"
#include "TDSInventoryComponent.generated.h"

class ATDSCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDS_API UTDSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTDSInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<ATDSWeaponBase>> WeaponInventory;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Inventory")
	TArray<FItemSettings> InventoryArray;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Inventory")
	TArray<TSubclassOf<ATDSDiary>> DiaryArray;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FOnPlayerFindItem OnPlayerFindItem;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FOnPlayerFindWeapon OnPlayerFindWeapon;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FOnPlayerFindDiary OnPlayerFindDiary;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FOnBulletsEnd OnBulletsEnd;
	UPROPERTY(BlueprintAssignable)
	FOnPickupItem OnPickupItem;
	UPROPERTY(BlueprintAssignable)
	FOnReceiveItem OnReceiveItem;
	UPROPERTY(BlueprintAssignable)
	FOnPressed_T OnPressed_T;
	UPROPERTY(BlueprintReadWrite)
	bool bIsCanPickupTo = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCanOpenDoor = false;

	void AddItem(ATDSItemBase* Item);
	void AddDiaryItem(ATDSDiary* Item);
	void AddWeaponItem(ATDSWeaponBase* Item);
	
	UFUNCTION(BlueprintCallable)
	int FindItemById(int aId);
	
	bool CheckCount(int WeaponBulletId);
	void DecreaseCount(int WeaponID);
	void TryToGetWeaponType(ATDSWeaponBase* BaseWeapon);
	
	void AddAid();
	void AddKey(FItemSettings NewKey);
	
	bool IsCanPickup();
	int32 NumAid = 0;
	FItemSettings HealData;
	void UseAid();
	bool ManaIsNotEmpty();

private:
	ATDSCharacter* GetPlayerOwner() const;
	void SetNumAid(int32 NewAid);
	void SpawnSoundPickup(USoundBase* NewSound);
	void SpawnParticleFx(UParticleSystem* NewParticle);
};
