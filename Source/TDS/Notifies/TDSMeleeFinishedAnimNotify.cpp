// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSMeleeFinishedAnimNotify.h"

void UTDSMeleeFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnMeleePointNotify.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}