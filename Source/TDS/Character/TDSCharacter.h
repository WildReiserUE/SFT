#pragma once

#include "CoreMinimal.h"
#include "TDSWeaponBase.h"
#include "GameFramework/Character.h"
#include "Types.h"
#include "TDSCharacter.generated.h"

UCLASS()
class ATDSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDSCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetCharacterCameraComponent() const { return CharacterCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraArm; }
	/** Returns SprintComponent subobject **/
	FORCEINLINE UTDSPlayerStateComponent* GetPlayerStateComponent() { return PlayerStateInfo; }

	UFUNCTION()
		void InputAxisY(float Value);
	UFUNCTION()
		void InputAxisX(float Value);
	UFUNCTION()
		void ActivateSprint();	
	UFUNCTION()
		void DeActivateSprint();
	UFUNCTION()
		UDecalComponent* GetCursorToWorld();
	UFUNCTION()
		ATDSWeaponBase* SpawnWeapon(int WeaponIndex);
	UFUNCTION()
		void FireOn();
	
	UFUNCTION()
		void FireOff();
	
	UFUNCTION()
		void DecreaseBulletCount();

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FOnPlayerWeaponSwitch OnPlayerWeaponSwitch;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* CursorToWorld;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BloodDecal;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Cursor")
	UMaterialInterface* CursorMaterial =nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Cursor")
	FVector CursorSize = FVector(20.0f,40.0f,40.0f);	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimMontageHandleAttack = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimMontageStartSkill = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimMontageDropSkill = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerInfo", meta = (AllowPrivateAccess = "true"))
	UTDSPlayerStateComponent* PlayerStateInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerInventory", meta = (AllowPrivateAccess = "true"))
	UTDSInventoryComponent* PlayerInventory;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon")
	ATDSWeaponBase* CurrentWeapon = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	float MeshDirection = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentCharSpeed = 0.0f;	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	bool bIsALive = true;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCanPickupToDiary = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsPickupToDiary = false;

	void PrevWeapon();
	void NextWeapon();
	void SkillBearPickup();
	void SkillBearDrop();
	UFUNCTION(BlueprintCallable)
	void DetachItem();
	void SetMana(float NewMana);
	bool ManaIsNotEmpty();

	UFUNCTION()
		void OnPlayerBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnPlayerHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	float AxisX = 0.0f;
	float AxisY = 0.0f;
	bool bSprintAllow = true;
	bool bCanMove = true;
	int CurrentWeaponIndex = -1;
	bool bSprintActivate;	

	UPROPERTY(BlueprintReadOnly)
	ETeamIDType NewTeamID = ETeamIDType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETeamIDType DefaultTeamID = ETeamIDType::None;
	
	FTimerHandle SkillPickUp;
	FTimerHandle SkillAttack;
	FTimerHandle WeaponReloadTimer;

	UPROPERTY(BlueprintReadOnly)
	bool bCanUseSkill = false;
	UPROPERTY(BlueprintReadOnly)
	bool bObjectPicked = false; 
	bool bFindInteract = false;
	UPROPERTY(BlueprintReadOnly)
	bool bPickPressed = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsGoodEnd = false;

	bool bIsSwitchEnable = false;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArm;

	void SwitchKey();
	void PickupToDiary();
	void StopPickupToDiary();
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnChangeStory OnChangeStory;
};
