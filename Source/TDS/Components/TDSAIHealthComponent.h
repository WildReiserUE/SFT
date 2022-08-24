// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types.h"
#include "TDSAIHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDS_API UTDSAIHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDSAIHealthComponent();
	FOnAIDeath OnAIDeath;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	float GetHealth() const { return Health; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "HealthSetting")
	bool bIsHaveHealth = false;

	UPROPERTY(EditDefaultsOnly, Category = "HealthSetting", meta = (EditCondition = "bIsHaveHealth"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AddHealthEnemy")
	float HealForEnemy = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "ColorSetting")
	FLinearColor FirstColor;

	UPROPERTY(EditDefaultsOnly, Category = "ColorSetting")
	FLinearColor SecondColor;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	UMaterialInterface* BloodDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	FVector DecalSize = FVector(50.0f, 50.0f, 50.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	FRotator DecalRotation;

	UPROPERTY(EditDefaultsOnly, Category = "DamageFireEverySecond")
	bool bIsEnemy = false;

	UPROPERTY(EditDefaultsOnly, Category = "DamageFireEverySecond", meta = (EditCondition = "bIsEnemy"))
	int32 MaxTimer = 5;

	UPROPERTY(EditDefaultsOnly, Category = "DamageFireEverySecond", meta = (EditCondition = "bIsEnemy"))
	float TimerBetween = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "DamageFireEverySecond", meta = (EditCondition = "bIsEnemy"))
	float BaseDamage = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "DamageFireEverySecond", meta = (EditCondition = "bIsEnemy"))
	UParticleSystem* FireEnemyFX = nullptr;

	AActor* Owner = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "DamagedSettingFX")
	UParticleSystem* FireFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "DamagedSettingFX")
	UParticleSystem* ExplosionFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "DamagedSettingFX")
	USoundBase* FireSound = nullptr;

private:
	float Health = 100.0f;

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void ColorChange();
	void OnFireDamage();
	void SetHealth(float CurrentHealth);
	void ApplySlowAbility(AActor* Target);
	void AddHealthEnemy(AActor* Target);

	FTimerHandle ColorTimerHandle;
	FTimerHandle FireTimerHandle;
	UParticleSystemComponent* ParticleSystemComp = nullptr;
	bool bIsColorChange = false;
	int32 Counter = 4;
	int32 TimerLeft = 0;
};
