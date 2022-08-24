// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSInventoryComponent.h"
#include "TDSCharacter.h"
#include "TDSDiary.h"
#include "TDSPlayerStateComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TDSPlayerController.h"

const float MinusMana = 100.0f;
const int NewAidValue = 1;

// Sets default values for this component's properties
UTDSInventoryComponent::UTDSInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UTDSInventoryComponent::AddItem(ATDSItemBase* Item)
{
	int i = FindItemById(Item->ItemInfo.ID);
	if (i == INDEX_NONE) //если элемента нет
	{
		FItemSettings NewItem;
		NewItem.ID = Item->ItemInfo.ID;
		NewItem.Count = Item->ItemInfo.Count;
		InventoryArray.Add(NewItem);
		ATDSWeaponBase* Weapon = Cast<ATDSWeaponBase>(Item);
		ATDSDiary* Diary = Cast<ATDSDiary>(Item);
		if(Weapon)
		{
			TryToGetWeaponType(Weapon);
			AddWeaponItem(Weapon);
		}
		else if(Diary)
		{
			AddDiaryItem(Diary);
		}
		SpawnSoundPickup(Item->ItemInfo.PickupSound);
		SpawnParticleFx(Item->ItemInfo.PickupFX);
		OnPlayerFindItem.Broadcast();
	}
	else
	{
		if (Item->ItemInfo.bItemStack) //складываемость
     	{
     		InventoryArray[i].Count += Item->ItemInfo.Count;
     	}
		UE_LOG(LogTemp,Warning,TEXT("ITEM PICKUPED"));
		OnPlayerFindItem.Broadcast();
	}
	SpawnSoundPickup(Item->ItemInfo.PickupSound);
	SpawnParticleFx(Item->ItemInfo.PickupFX);
}

void UTDSInventoryComponent::SpawnSoundPickup(USoundBase* NewSound)
{
	if (!GetWorld() || !NewSound) return;
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NewSound, GetOwner()->GetActorLocation());
}

void UTDSInventoryComponent::SpawnParticleFx(UParticleSystem* NewParticle)
{
	if(!GetWorld() || !NewParticle) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NewParticle, GetOwner()->GetActorLocation());
}

void UTDSInventoryComponent::TryToGetWeaponType(ATDSWeaponBase* BaseWeapon)
{
	if (BaseWeapon->WeaponInfo.WeaponType == EWeaponType::WeaponKey)
	{
		//bIsCanOpenDoor = true;
		OnPickupItem.Broadcast(BaseWeapon);
	}
}

void UTDSInventoryComponent::AddDiaryItem(ATDSDiary* Item)
{
	DiaryArray.Add(Item->GetClass());
	OnPlayerFindDiary.Broadcast();
}

void UTDSInventoryComponent::AddWeaponItem(ATDSWeaponBase* Item)
{
	WeaponInventory.Add(Item->GetClass());
	OnPlayerFindWeapon.Broadcast();
}

void UTDSInventoryComponent::DecreaseCount(int WeaponBulletId)
{	
	int i = FindItemById(WeaponBulletId);
	if (i == INDEX_NONE) //если элемента нет
	{
		//UE_LOG(LogTemp,Warning,TEXT("FIRE BULLET HET COBCEM"));
		OnBulletsEnd.Broadcast();
	}
	else
	{
		if(InventoryArray[i].Count >= 1)
		{
			//UE_LOG(LogTemp,Warning,TEXT("FIRE BULLET ECTb --- SHOT --- %d"), InventoryArray[i].Count);
			InventoryArray[i].Count -= 1;
			if(InventoryArray[i].Count == 0)
			{
				InventoryArray.RemoveAt(i);
				OnBulletsEnd.Broadcast();
			}
		}
		else //по логике сюда не зайдем никогда... но мало ли...
		{
			//UE_LOG(LogTemp,Warning,TEXT("FIRE BULLET ECTb HO SHOT = 0"));
			OnBulletsEnd.Broadcast();
		}
	}
}

bool UTDSInventoryComponent::CheckCount(int WeaponBulletId)
{
	bool BulletAviable = false;
	int InventoryBulletCount = 0;
	int i = FindItemById(WeaponBulletId);
	if (i == INDEX_NONE) //если элемента нет
	{
		//UE_LOG(LogTemp,Warning,TEXT("CHECK BULLET --- HET COBCEM"));
		BulletAviable = false;
	}
	else
	{
		if(InventoryArray[i].Count >= 1) // рабочий элемент, нашли
		{
			//UE_LOG(LogTemp,Warning,TEXT("CHECK BULLET --- ECTb --- %d --- COUNT "), InventoryArray[i].Count);
			InventoryBulletCount = InventoryArray[i].Count;
			BulletAviable =  true;
		}
		else // элемент есть но пустой
		{
			//UE_LOG(LogTemp,Warning,TEXT("CHECK BULLET --- ECTb HO COUNT = 0"));
			BulletAviable = false;
			InventoryBulletCount = 0;
		}
	}
	return BulletAviable;
}

int UTDSInventoryComponent::FindItemById(int aId)
{
	int n = INDEX_NONE;
	int i = 0;
	for (FItemSettings aItem : InventoryArray)
	{
		if (aItem.ID == aId)
		{
			n = i;
			break;
		}
		i++;
	}
	//UE_LOG(LogTemp, Warning, TEXT("INVENTORY RETURN N - %d"), n);
	return n;
}

void UTDSInventoryComponent::AddAid()
{
	if (!IsCanPickup()) return;
	//UE_LOG(LogTemp, Warning, TEXT("Aid was Add!"));
	const auto Player = GetPlayerOwner();
	Player->SetMana(-MinusMana);
	SetNumAid(NewAidValue);
	OnReceiveItem.Broadcast();
	//UE_LOG(LogTemp, Warning, TEXT("Aid = %i"), HealData.Count);
}

bool UTDSInventoryComponent::IsCanPickup()
{
	return bIsCanPickupTo && HealData.Count < 1 && ManaIsNotEmpty();
}

void UTDSInventoryComponent::UseAid()
{
	const auto Owner = Cast<ATDSCharacter>(GetOwner());
	if (!Owner) return;

	const auto Controller = Cast<ATDSPlayerController>(Owner->GetController());

	if ((Controller && !Controller->bEnableClickEvents) || !Owner->bCanUseSkill)
	{
		Controller->bEnableClickEvents = true;
		Owner->bCanUseSkill = true;
	}
	OnPressed_T.Broadcast();

	const auto HealthComponent = Owner->FindComponentByClass<UTDSPlayerStateComponent>();
	if (HealData.Count == 0 || !HealthComponent || HealthComponent->IsDead() || HealthComponent->GetHealth() == 100.0f) return;
	
	SetNumAid(-NewAidValue);
	//UE_LOG(LogTemp, Warning, TEXT("Aid was Used!"));
	//UE_LOG(LogTemp, Warning, TEXT("Aid = %i"), HealData.Count);

	HealthComponent->RefreshHealth(100.0f);
}

bool UTDSInventoryComponent::ManaIsNotEmpty()
{
	const auto Player = GetPlayerOwner();
	return Player->ManaIsNotEmpty();
}

ATDSCharacter* UTDSInventoryComponent::GetPlayerOwner() const
{
	const auto Owner = Cast<ATDSCharacter>(GetOwner());
	if (!Owner) return nullptr;

	return Owner;
}

void UTDSInventoryComponent::SetNumAid(int32 NewAid)
{
	HealData.Count = FMath::Clamp(HealData.Count + NewAid, 0, 1);
}

