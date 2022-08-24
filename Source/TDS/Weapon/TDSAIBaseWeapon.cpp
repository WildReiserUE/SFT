
#include "TDSAIBaseWeapon.h"
#include "TDSProjectileBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

ATDSAIBaseWeapon::ATDSAIBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetupAttachment(GetRootComponent());
}

void ATDSAIBaseWeapon::StartFire()
{
	if (!GetWorld() || !GetOwner()) return;
	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ATDSAIBaseWeapon::Shot, RateOfFire, true, 0.0f);
}

void ATDSAIBaseWeapon::StopFire()
{
	if (!GetWorld() || !GetOwner()) return;
	GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
}

void ATDSAIBaseWeapon::Shot()
{
	if (!GetWorld() || !ProjectileClass) return;

	FHitResult Hit;
	const FVector SpawnLocation = WeaponMesh->GetSocketTransform("MuzzleFlashSocket").GetLocation();
	const FRotator SpawnRotation = GetActorRotation();
		//FRotator(0.0f, (WeaponMesh->GetSocketRotation("MuzzleFlashSocket").Yaw), 0.0f);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = GetInstigator();
	const auto Projectile = GetWorld()->SpawnActor<ATDSProjectileBase>(ProjectileClass, WeaponMesh->GetSocketTransform("MuzzleFlashSocket"));
	if (Projectile)
	{
		SpawnMuzzleFX();
	}
	GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
	OnShot.Broadcast();
}

void ATDSAIBaseWeapon::SpawnMuzzleFX()
{
	if (!ShotParticle || ShotSounds.Num() == 0) return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShotParticle, WeaponMesh->GetSocketLocation("MuzzleFlashSocket"));

	int32 Rand = FMath::RandRange(0, ShotSounds.Num() - 1);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShotSounds[Rand], WeaponMesh->GetSocketLocation("MuzzleFlashSocket"));
}