// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSDistantAttackCharacter.h"
#include "TDSAIBaseWeapon.h"
#include "Components/TDSAIHealthComponent.h"


void ATDSDistantAttackCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
}

void ATDSDistantAttackCharacter::SpawnWeapon()
{
	if (!GetWorld() || !GetMesh() || !WeaponClass) return;

	CurrentWeapon = GetWorld()->SpawnActor<ATDSAIBaseWeapon>(WeaponClass);
	if (!CurrentWeapon) return;

	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(GetMesh(), TransformRules, "RightHandSocket");
	CurrentWeapon->SetOwner(this);
	CurrentWeapon->OnShot.AddUObject(this, &ATDSDistantAttackCharacter::OnShot);
}

void ATDSDistantAttackCharacter::StartFire()
{
	if (!CurrentWeapon || AIHealthComponent->IsDead()) return;
	CurrentWeapon->StartFire();
}

void ATDSDistantAttackCharacter::StopFire()
{
	if (!CurrentWeapon || AIHealthComponent->IsDead()) return;
	CurrentWeapon->StopFire();
}

void ATDSDistantAttackCharacter::OnDead()
{
	Super::OnDead();
	CurrentWeapon->Destroy();
}

void ATDSDistantAttackCharacter::OnShot()
{
	if (!ShotAnimMontage) return;
	PlayAnimMontage(ShotAnimMontage);
}