// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types.h"
#include "TDSAIBaseCharacter.generated.h"

class UBehaviorTree;
class UTDSAIHealthComponent;
class ATDSItemBase;

UCLASS()
class TDS_API ATDSAIBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ATDSAIBaseCharacter();

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStateOfCalm = false;

	UPROPERTY(BlueprintAssignable)
	FOnBossDeath OnBossDeath;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTDSAIHealthComponent* AIHealthComponent;

	UPROPERTY(EditDefaultsOnly)
	TArray<UAnimMontage*> DeadAnimMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Drop Probability")
	float SuccessRate = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Drop Probability")
	TArray<TSubclassOf<ATDSItemBase>> ItemClasses;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxTimerLeft = 0;

	virtual void BeginPlay() override;
	virtual void OnDead();
	FOnAIDeath OnAIDeath;
public:	

	void UpdateVelocityMovement(float SlowSpeedValue);
	virtual void Attack();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TeamID")
	ETeamIDType DefaultTeamID = ETeamIDType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "TeamID")
	ETeamIDType NewTeamID = ETeamIDType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BotName")
	FName BotName = "None";
private:
	void MakeRandomItemDrop();

	FTimerHandle SlowTimeHandle;
	void SlowDown();
	int32 TimerLeft = 0;
	float DefaultWalkSpeed = 0.0f;

	void BossIsDead();
};
