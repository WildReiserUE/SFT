#include "TDSPlayerStateComponent.h"
#include "TDSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTDSPlayerStateComponent::UTDSPlayerStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Health = HealthSetting.MaxHealth;
	Stamina = StaminaSetting.MaxStamina;
	MaxStamina = StaminaSetting.MaxStamina;
	Mana = ManaSetting.MaxMana;
}


void UTDSPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();
	const auto ComponentOwner = GetOwner();
	if (!ComponentOwner) return;

	ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UTDSPlayerStateComponent::TakeAnyDamage);
}

void UTDSPlayerStateComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	static ATDSCharacter* PlayerInstig = Cast<ATDSCharacter>(InstigatedBy);
	static ATDSCharacter* PlayerCauser = Cast<ATDSCharacter>(DamageCauser);
	
	if(PlayerInstig || PlayerCauser) return;
	
	//UE_LOG(LogTemp, Warning, TEXT("CHARACTER HEALTH %f"), Damage);
	SetHealth(-Damage);
}

void UTDSPlayerStateComponent::UpdateHealth()
{
	OnHealthChange.Broadcast(Health, HealthSetting.MaxHealth);
	//UE_LOG(LogTemp, Warning, TEXT("CHARACTER HEALTH %f"), Health);
}

void UTDSPlayerStateComponent::StartSprint()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(StaminaRecoveryTimer))
		GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaLoseTimer,this, &UTDSPlayerStateComponent::MovementStaminaLose, StaminaSetting.TimerBetween, true,0.0f);
	OnStaminaChange.Broadcast(Stamina,StaminaSetting.MaxStamina);
}

void UTDSPlayerStateComponent::MovementStaminaLose()
{
	static ATDSCharacter* Player = Cast<ATDSCharacter>(GetOwner());
	if(Player->AxisX !=0 || Player->AxisY !=0)
	{
		if(Stamina<StaminaSetting.DecreaseValue*SprintCoef)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaLoseTimer);
			Stamina=0.0f;
			OnStaminaChange.Broadcast(Stamina,StaminaSetting.MaxStamina);
		
			if(Player)
			{
				//UE_LOG(LogTemp, Warning, TEXT("CHARACTER FOUND"));
				Player->DeActivateSprint();
				StartStaminaRefresh();
			}
		
		}
		else
		{
			Stamina -= (StaminaSetting.DecreaseValue*SprintCoef);
			OnStaminaChange.Broadcast(Stamina,StaminaSetting.MaxStamina);
		}
	}	
}

void UTDSPlayerStateComponent::StartStaminaRefresh()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaLoseTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaRecoveryTimer,this, &UTDSPlayerStateComponent::StartStaminaRepair, StaminaSetting.TimerBetween, true,CurrentStaminaSettings.StartTimer);		
}

void UTDSPlayerStateComponent::StartStaminaRepair()
{
	if(Stamina<StaminaSetting.MaxStamina)
	{
		if(GetWorld() && Stamina>=StaminaSetting.MaxStamina)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimer);
			Stamina = StaminaSetting.MaxStamina;
			bSprintStart = false;
		}
		Stamina += StaminaSetting.IncreaseValue;
		OnStaminaChange.Broadcast(Stamina,StaminaSetting.MaxStamina);
	}
}

void UTDSPlayerStateComponent::TryKillSelf()
{
	if(!GetWorld()->GetTimerManager().IsTimerActive(KillSelfTimer))
		GetWorld()->GetTimerManager().SetTimer(KillSelfTimer,this, &UTDSPlayerStateComponent::TimerToDeath, 2.0f, true,10.0f);
}

void UTDSPlayerStateComponent::TimerToDeath()
{
	if(Health>0)
	{
		Health -= 1;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("DEATH IS COMMING, HEALTH = %f"), Health));
		UpdateHealth();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(KillSelfTimer);
		OnPlayerDeath.Broadcast();
	}
}

void UTDSPlayerStateComponent::StartUseSkill()
{
	bSkillStarted = true;
	Mana -= 20;
	OnManaChange.Broadcast(Mana, ManaSetting.MaxMana);
}

void UTDSPlayerStateComponent::RecoverMana()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(ManaTimerHandle))
		GetWorld()->GetTimerManager().ClearTimer(ManaTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ManaTimerHandle,this,&UTDSPlayerStateComponent::StartManaRepair,ManaSetting.TimerBetween,true,ManaSetting.StartTimer);
}

void UTDSPlayerStateComponent::StartManaRepair()
{
	if(Mana<ManaSetting.MaxMana)
	{
		Mana += ManaSetting.IncreaseValue;		
		if(Mana>=ManaSetting.MaxMana)
		{
			GetWorld()->GetTimerManager().ClearTimer(ManaTimerHandle);
			Mana = ManaSetting.MaxMana;
		}
		OnManaChange.Broadcast(Mana,ManaSetting.MaxMana);
	}
}

void UTDSPlayerStateComponent::ChangeManaValue(float NewMana)
{
	Mana = FMath::Clamp(Mana + NewMana, 0.0f, ManaSetting.MaxMana);
	OnManaChange.Broadcast(Mana,ManaSetting.MaxMana);
	RecoverMana();
}

void UTDSPlayerStateComponent::SetHealth(float NewHealth)
{	
	Health = FMath::Clamp(Health + NewHealth, 0.0f, HealthSetting.MaxHealth);	
	if (Health <= 0.0f)
	{
		UpdateHealth();
		OnPlayerDeath.Broadcast();
	}
	else
	{
		UpdateHealth();
	}
}

void UTDSPlayerStateComponent::RefreshHealth(float NewHealth)
{
	const auto Player = Cast<ATDSCharacter>(GetOwner());
	if (!GetWorld() || !HealFX || !Player) return;

	UGameplayStatics::SpawnEmitterAttached(HealFX, Player->GetRootComponent());
	SetHealth(NewHealth);
}
