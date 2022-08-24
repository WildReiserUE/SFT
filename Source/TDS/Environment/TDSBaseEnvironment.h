// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDSBaseEnvironment.generated.h"

class UTDSAIHealthComponent;
class UStaticMeshComponent;

UCLASS()
class TDS_API ATDSBaseEnvironment : public AActor
{
	GENERATED_BODY()
	
public:

	ATDSBaseEnvironment();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshObject = nullptr;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UTDSAIHealthComponent* AIHealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion")
	bool bIsActiveTimerDamage = false;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion", meta = (EditCondition = "!bIsActiveTimerDamage"))
	float Damage = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion", meta = (EditCondition = "!bIsActiveTimerDamage"))
	float DamageRadius = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion", meta = (EditCondition = "bIsActiveTimerDamage"))
	float DamageEverySecond = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion", meta = (EditCondition = "bIsActiveTimerDamage"))
	float TimerBetween = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion", meta = (EditCondition = "bIsActiveTimerDamage"))
	int32 TimerLeft = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion", meta = (EditCondition = "bIsActiveTimerDamage"))
	float DamageRadiusFire = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setting Explosion", meta = (EditCondition = "bIsActiveTimerDamage"))
	UParticleSystem* FireFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion", meta = (EditCondition = "bIsActiveTimerDamage"))
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Setting Explosion")
	USoundBase* SoundExplosion = nullptr;

private:
	void OnDead();
	void TimerDamage();

	FTimerHandle ExplosionTimerHandle;

};
