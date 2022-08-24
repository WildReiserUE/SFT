// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSWeaponBase.h"

#include "TDSAIBaseCharacter.h"
#include "TDSAIHealthComponent.h"
#include "TDSProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATDSWeaponBase::ATDSWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphereCollision"));	
	AttackSphereComponent->SetupAttachment(StaticMeshItem);
	AttackSphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ATDSWeaponBase::ApplyWeaponDamage);
	AttackSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackSphereComponent->InitSphereRadius(5);
}

bool ATDSWeaponBase::CheckWeaponCanFire()
{
	return WeaponInfo.WeaponCanFire;
}

float ATDSWeaponBase::CalculateWeaponDamage()
{
	if(WeaponInfo.WeaponType == EWeaponType::WeaponKey)
	{
		return WeaponInfo.WeaponDamage;
	}
	return 0.0f;
}

void ATDSWeaponBase::ApplyWeaponDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const ATDSAIBaseCharacter* Target = Cast<ATDSAIBaseCharacter>(OtherActor);
	if(Target)
	{
		//UE_LOG(LogTemp,Warning,TEXT("WEAPON HIT BY --- %s"), *OtherActor->GetClass()->GetName());
		UGameplayStatics::ApplyDamage(OtherActor, WeaponInfo.WeaponDamage, GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());
	}
}

void ATDSWeaponBase::UpdateWeaponType(EWeaponType WType)
{
	WeaponInfo.WeaponType = WType;
}

void ATDSWeaponBase::Fire()
{
	if(!bIsMousePressed && !(GetWorld()->GetTimerManager().IsTimerActive(WeaponFireTimer)))
	{
		GetWorld()->GetTimerManager().SetTimer(WeaponFireTimer,this, &ATDSWeaponBase::SpawnBullet,WeaponInfo.WeaponFireLag, true,0.0f);
		bIsMousePressed = true;
	}
}

void ATDSWeaponBase::StopFire()
{
	bIsMousePressed = false;
}

void ATDSWeaponBase::SpawnBullet()
{
	
	if(!bIsMousePressed)
	{
		GetWorld()->GetTimerManager().ClearTimer(WeaponFireTimer);
		bIsMousePressed = false;
		//UE_LOG(LogTemp,Warning,TEXT("WEAPON LOGIC 1 END"));
	}
	else
	{		
		if(CheckWeaponCanFire() && WeaponInfo.WeaponBullet < 0)
		{
			if(WeaponInfo.ShotSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),WeaponInfo.ShotSound, GetOwner()->GetActorLocation());
			}
			//UE_LOG(LogTemp,Warning,TEXT("WEAPON LOGIC 2 ELSE"));
			FVector SpawnLocation = RootComponent->GetSocketLocation(FName("BulletSocket"));
			FRotator SpawnRotation = FRotator(0.0f,(this->GetActorRotation().Yaw), 0.0f);	
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetInstigator();	
			ATDSProjectileBase* myProjectile = Cast<ATDSProjectileBase>(GetWorld()->SpawnActor(WeaponInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawnParams));
			OnWeaponFire.Broadcast();
		}
		else if(!CheckWeaponCanFire())
		{
			if(WeaponInfo.ShotSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),WeaponInfo.ShotSound, GetOwner()->GetActorLocation());
			}
			//UE_LOG(LogTemp,Warning,TEXT("WEAPON LOGIC 3 ELSE"));
		}
		else
		{
			//UE_LOG(LogTemp,Warning,TEXT("WEAPON LOGIC 4 ELSE"));
			for (int i=0;i<WeaponInfo.WeaponShotBullet;i++)
			{
				FVector SpawnLocation = RootComponent->GetSocketLocation(FName("BulletSocket"));
				float BaseY = this->GetActorRotation().Yaw;
				float BaseX = this->GetActorRotation().Pitch;
				float RndY = UKismetMathLibrary::RandomFloatInRange(-20.0f,+20.0f);
				float RndX = UKismetMathLibrary::RandomFloatInRange(-10.0f,+10.0f);
				FRotator SpawnRotation = FRotator(BaseX + RndX,BaseY + RndY, 0.0f);
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();
				ATDSProjectileBase* myProjectile = Cast<ATDSProjectileBase>(GetWorld()->SpawnActor(WeaponInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawnParams));
			}
			if(WeaponInfo.ShotSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),WeaponInfo.ShotSound, GetOwner()->GetActorLocation());
			}
			OnWeaponFire.Broadcast();			
		}
	}
}