// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDSAIBaseCharacter.h"
#include "Types.h"
#include "TDSAIMeleeCharacter.generated.h"

class USphereComponent;

UCLASS()
class TDS_API ATDSAIMeleeCharacter : public ATDSAIBaseCharacter
{
	GENERATED_BODY()
	
public:
	ATDSAIMeleeCharacter();
	virtual void Attack() override;
	virtual void BeginPlay() override;

	int32 IsCanMelee() const;
	
protected:

	void InitAnimations();
	virtual void OnMeleePoint(USkeletalMeshComponent* SkeletalMesh);
	virtual void OnMeleeFinished(USkeletalMeshComponent* SkeletalMesh);
	virtual void OnDead() override;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackAnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float RadiusDamage = 120.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float RateOfAttack = 3.0f;


	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FTimerHandle MeleeTimerHandle;

	bool bIsPunch = true;
	int32 number = 0;
	bool bIsMeleeFinished = true;
};
