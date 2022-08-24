// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSMeleePointAnimNotify.h"

void UTDSMeleePointAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnMeleePointNotify.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}