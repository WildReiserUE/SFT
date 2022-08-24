#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "TDSInteractable.generated.h"

UCLASS()
class TDS_API ATDSInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDSInteractable();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnItemBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnClicked(UPrimitiveComponent* Component, FKey FKey);

	UFUNCTION()
	void ItemParamsDefault();
	UFUNCTION()
	void ItemParamsHit();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Collision", meta=(AllowPrivateAccess="true"))
	UBoxComponent* ItemHitCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HitFX")
	UParticleSystem* HitEnemyFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ItemMovement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HitEnemy")
	USoundCue* HitEnemy = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HitStatic")
	USoundCue* HitStatic = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="HitStatic")
	USoundCue* SoundMove = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	float Damage = 0.0f;

	bool bItemIsPickuped = false;
	bool bIsFirstHit = false;
};
