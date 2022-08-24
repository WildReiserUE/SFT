#pragma once
#include "Types.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health,float, MaxHealthValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChange, float, Stamina,float, MaxStaminaValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChange, float, Mana, float, MaxManaVaule);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickupItem, class ATDSWeaponBase*, WeaponBase);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMeleePointNotify, class USkeletalMeshComponent*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMeleeFinishedNotify, class USkeletalMeshComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossDeath, FName, BotName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeStory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerFindItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerFindDiary);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerFindWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReceiveItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerWeaponSwitch);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBulletsEnd);
DECLARE_MULTICAST_DELEGATE(FOnShot);
DECLARE_MULTICAST_DELEGATE(FOnAIDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressed_T);

class ATDSProjectileBase;
class UStaticMeshComponent;
class UDecalComponent;
class UAnimMontage;
class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UTDSPlayerStateComponent;
class UTDSInventoryComponent;
class UParticleSystem;
class UParticleSystemComponent;
class USoundCue;
class USoundBase;

UENUM()
enum class EWeaponType:uint8
{
	NoWeapon,
	WeaponKey,
	WeaponAK,
	WeaponSguha
};

USTRUCT(BlueprintType)
struct FHealthSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Heal = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StartTimer = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimerBetween = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DecreaseValue = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DecreaseDelay = 2.0f;
};

USTRUCT(BlueprintType)
struct FStaminaSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DecreaseValue = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float IncreaseValue = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StartTimer = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimerBetween = 0.1f;
};

USTRUCT(BlueprintType)
struct FManaSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxMana = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float IncreaseValue = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StartTimer = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimerBetween = 0.1f;
};

USTRUCT(BlueprintType)
struct FWeaponSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponDamage = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WeaponFireLag = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int WeaponBullet = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int WeaponShotBullet = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int BulletItemId = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool WeaponCanFire = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATDSProjectileBase> Projectile = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* WeaponIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ShotSound = nullptr;
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	NONE,
	Key,
	Heal
};

USTRUCT(BlueprintType)
struct FDiarySettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TextButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TextMission;
};

USTRUCT(BlueprintType)
struct FItemSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int	ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int	Count;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bItemStack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType DefaultItemType = EItemType::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* PickupSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* PickupFX = nullptr;
};

UENUM()
enum class EProjectileType : uint8
{
	NONE	  UMETA(DisplayName = "NONE"),
	Bullet    UMETA(DisplayName = "Bullet"),
	Condensed UMETA(DisplayName = "Condensed")
};

USTRUCT(BlueprintType)
struct FProjectileSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EProjectileType DefaultProjectileType = EProjectileType::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BulletSlowTargetBy = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProjectileDamage = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeToDestroy = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bItemStack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* HitFX = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ApplySlowFX = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* BloodFX = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Force = 50.f;
};

UENUM(BlueprintType)
enum class EStateAffilation : uint8
{
	Neutral,
	Enemy,	
	Target,
	None
};

UENUM(BlueprintType)
enum class ETeamIDType : uint8
{
	None,
	Team_1,
	Team_2,
	Neutral
};