// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSAIBaseCharacter.h"
#include "TDSAIHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/UnrealMathUtility.h"
#include "TDSItemBase.h"
#include "TDSPlayerState.h"
#include "TDSAIController.h"
#include "TDSGameMode.h"



ATDSAIBaseCharacter::ATDSAIBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIHealthComponent = CreateDefaultSubobject<UTDSAIHealthComponent>(TEXT("AIHealthComponent"));
	TimerLeft = MaxTimerLeft;
}

void ATDSAIBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(AIHealthComponent);
	
	AIHealthComponent->OnAIDeath.AddUObject(this, &ATDSAIBaseCharacter::OnDead);
}

void ATDSAIBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDSAIBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATDSAIBaseCharacter::Attack()
{

}

void ATDSAIBaseCharacter::OnDead()
{
	if (!GetMesh() || !GetWorld()) return;

	const auto GameMode = Cast<ATDSGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	GameMode->OneActorIsDead();

	if (DeadAnimMontages.Num() != 0)
	{
		const int32 DeadRandNum = FMath::RandRange(0, DeadAnimMontages.Num() - 1);
		PlayAnimMontage(DeadAnimMontages[DeadRandNum]);
	}
	MakeRandomItemDrop();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BossIsDead();
	UnPossessed();
	SetLifeSpan(3.0f);
}


void ATDSAIBaseCharacter::MakeRandomItemDrop()
{
	if (ItemClasses.Num() == 0) return;

	const float SuccessResult = FMath::FRandRange(0.0f, 100.0f);
	if (SuccessResult < SuccessRate) return;
		
	const int32 ItemRandomDropNum = FMath::RandRange(0, ItemClasses.Num() - 1);
	if (!ItemClasses.IsValidIndex(ItemRandomDropNum)) return;

	GetWorld()->SpawnActor<ATDSItemBase>(ItemClasses[ItemRandomDropNum], GetActorTransform());
}

void ATDSAIBaseCharacter::UpdateVelocityMovement(float SlowSpeedValue)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SlowTimeHandle)) return;

	TimerLeft = MaxTimerLeft;
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed *= SlowSpeedValue;
	GetWorld()->GetTimerManager().SetTimer(SlowTimeHandle, this, &ATDSAIBaseCharacter::SlowDown, 1.0f, true, 0.0f);
}

void ATDSAIBaseCharacter::SlowDown()
{
	if (--TimerLeft <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SlowTimeHandle);
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
		return;
	}
}

void ATDSAIBaseCharacter::BossIsDead()
{
	OnBossDeath.Broadcast(BotName);
}
