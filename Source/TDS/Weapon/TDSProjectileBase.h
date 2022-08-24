// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "TDSItemBase.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "TDSProjectileBase.generated.h"

UCLASS()
class TDS_API ATDSProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDSProjectileBase();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UParticleSystemComponent* ProjectileFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileSettings")
	FProjectileSettings ProjectileInfo;

	UFUNCTION()
	void ProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void TimerToDestroy();
	
	FProjectileSettings GetProjectileInfo() const { return ProjectileInfo; }

private:
	FTimerHandle BulletDestroyTimer;
	void MoveObject(UPrimitiveComponent* MeshObject);
	void SpawnBloodFX(const FHitResult& Hit);
	
};