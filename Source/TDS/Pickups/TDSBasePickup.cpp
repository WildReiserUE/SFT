
#include "Pickups/TDSBasePickup.h"
#include "Components/SphereComponent.h"

ATDSBasePickup::ATDSBasePickup()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereComponent->SetupAttachment(GetRootComponent());
	
}

