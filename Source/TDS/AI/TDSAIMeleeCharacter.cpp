// Fill out your copyright notice in the Description page of Project Settings.

#include "TDSAIMeleeCharacter.h"
#include "TDSMeleePointAnimNotify.h"
#include "TDSMeleeFinishedAnimNotify.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "TDSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TDSAIHealthComponent.h"
#include "TDSInteractable.h"
#include "Perception/AISense.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TDSAIController.h"
#include "TDSPlayerController.h"


ATDSAIMeleeCharacter::ATDSAIMeleeCharacter()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATDSAIMeleeCharacter::BeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATDSAIMeleeCharacter::OnEndOverlap);

}

void ATDSAIMeleeCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAnimations();
	MakeNoise();
}

void ATDSAIMeleeCharacter::InitAnimations()
{
	if (!AttackAnimMontage) return;

	const auto AnimNotifies = AttackAnimMontage->Notifies;
	for (const auto& AnimNotify : AnimNotifies)
	{
		const auto MeleePointNotify = Cast<UTDSMeleePointAnimNotify>(AnimNotify.Notify);
		const auto MeleeFinishedNotify = Cast<UTDSMeleeFinishedAnimNotify>(AnimNotify.Notify);
		if (MeleePointNotify)
		MeleePointNotify->OnMeleePointNotify.AddUObject(this, &ATDSAIMeleeCharacter::OnMeleePoint);

		if (MeleeFinishedNotify)
		MeleeFinishedNotify->OnMeleePointNotify.AddUObject(this, &ATDSAIMeleeCharacter::OnMeleeFinished);
	}
}

void ATDSAIMeleeCharacter::OnMeleePoint(USkeletalMeshComponent* SkeletalMesh)
{
	if (!GetMesh() || GetMesh() != SkeletalMesh) return;
	
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATDSAIBaseCharacter::StaticClass(), AllActors);

	TArray<AActor*> IgnoreActors;
	for (const auto& CurrentActor : AllActors)
	{
		const auto Pawn = Cast<ATDSAIBaseCharacter>(CurrentActor);
		if (!Pawn) break;
		
		if (Pawn->DefaultTeamID == DefaultTeamID || Pawn->DefaultTeamID == ETeamIDType::Neutral)
		{
			IgnoreActors.Add(Pawn);
		}
		
	}
	
	if (IgnoreActors.Num() == 0) return;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetMesh()->GetBoneLocation("RightHand"), RadiusDamage, {}, IgnoreActors, this, nullptr, true, ECollisionChannel::ECC_Visibility);
}

void ATDSAIMeleeCharacter::OnMeleeFinished(USkeletalMeshComponent* SkeletalMesh)
{
	if (!GetMesh() || GetMesh() != SkeletalMesh) return;
	bIsMeleeFinished = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ATDSAIMeleeCharacter::Attack()
{
	if (!GetWorld() || !AttackAnimMontage || AIHealthComponent->IsDead()) return;
	number = 1;
	PlayAnimMontage(AttackAnimMontage);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void ATDSAIMeleeCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || DefaultTeamID == ETeamIDType::Neutral) return;

	const auto Target = Cast<ATDSAIBaseCharacter>(OtherActor);
	const auto Target2 = Cast<ATDSCharacter>(OtherActor);
	

	if ((Target2 && DefaultTeamID != Target2->DefaultTeamID) || ((Target && Target->DefaultTeamID != DefaultTeamID) && (Target && Target->DefaultTeamID != ETeamIDType::Neutral)))
	{
		if (!AIHealthComponent->IsDead())
			GetWorld()->GetTimerManager().SetTimer(MeleeTimerHandle, this, &ATDSAIMeleeCharacter::Attack, RateOfAttack, true, 0.0f);
		else
			GetWorld()->GetTimerManager().ClearTimer(MeleeTimerHandle);
	}
	
}

void ATDSAIMeleeCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!GetWorld() || !Cast<APawn>(OtherActor)) return;

	const auto EnemyHealthComponent = OtherActor->FindComponentByClass<UTDSAIHealthComponent>();
	if (EnemyHealthComponent && EnemyHealthComponent->IsDead())
	{
		
	}
	number = 0;
	GetWorld()->GetTimerManager().ClearTimer(MeleeTimerHandle);
}

int32 ATDSAIMeleeCharacter::IsCanMelee() const
{
	return number;
}

void ATDSAIMeleeCharacter::OnDead()
{
	Super::OnDead();
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}
