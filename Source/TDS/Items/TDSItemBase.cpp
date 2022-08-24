// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSItemBase.h"

ATDSItemBase::ATDSItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	StaticMeshItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshItem->SetupAttachment(RootComponent);
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionCapsule->SetCapsuleRadius(5);
	CollisionCapsule->SetCapsuleHalfHeight(5);
	CollisionCapsule->SetupAttachment(StaticMeshItem);
}