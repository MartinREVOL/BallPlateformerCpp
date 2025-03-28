#include "Connecteur.h"

AConnecteur::AConnecteur()
{
	PrimaryActorTick.bCanEverTick = true;

	ConnectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConnectorMesh"));
	RootComponent = ConnectorMesh;

	ConnectorMesh->SetSimulatePhysics(true);
	ConnectorMesh->SetMobility(EComponentMobility::Movable);
}

void AConnecteur::BeginPlay()
{
	Super::BeginPlay();
}

void AConnecteur::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
