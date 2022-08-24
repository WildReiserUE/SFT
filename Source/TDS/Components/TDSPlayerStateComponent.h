// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types.h"
#include "TDSPlayerStateComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDS_API UTDSPlayerStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTDSPlayerStateComponent();

	virtual void BeginPlay() override;
	float GetHealth() const { return Health; }

	UPROPERTY(BlueprintAssignable)
	FOnHealthChange OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChange OnStaminaChange;

	UPROPERTY(BlueprintAssignable)
	FOnManaChange OnManaChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeath;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return Health == 0.0f; }

	void RefreshHealth(float NewHealth);

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	FHealthSettings HealthSetting;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
	FStaminaSettings StaminaSetting;
	
	UPROPERTY(EditDefaultsOnly, Category = "Mana")
	FManaSettings ManaSetting;

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FTimerHandle LoseHealthTimerHandle;
	FTimerHandle StaminaTimerHandle;
	FTimerHandle ManaTimerHandle;

	bool bIsHit = false;
	
private:
	
	// UFUNCTION(BlueprintCallable, Category="Health")
	// void ReciveDamage(float Damage);

	FHealthSettings CurrentHealthSettings;	
	FStaminaSettings CurrentStaminaSettings;
	FManaSettings CurrentManaSettings;
	
	FTimerHandle StaminaRecoveryTimer;
	FTimerHandle StaminaLoseTimer;
	FTimerHandle KillSelfTimer;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void UpdateHealth();	
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	bool NeedHealth = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category="Stamina")
	bool bCanSprint = false;	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category="Stamina")
	bool bSprintStart = false;	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category="Stamina")
	float SprintCoef = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "HealCharacterFX")
	UParticleSystem* HealFX = nullptr;
	
	void StartSprint();

	void MovementStaminaLose();

	void StartStaminaRefresh();

	void StartStaminaRepair();
	
	UFUNCTION(BlueprintCallable)
	void TryKillSelf();
	
	void TimerToDeath();
	void StartUseSkill();
	void RecoverMana();
	void StartManaRepair();
	void ChangeManaValue(float NewMana);
	void SetHealth(float NewHealth);
	//void ReceiveHeal(float Heal);
	float GetMana() const { return Mana; }
	
	float Health = 0.0f;
	float Stamina = 0.0f;
	float Mana = 0.0f;
	float MaxStamina = 0.0f;
	bool bSkillStarted = false;
};