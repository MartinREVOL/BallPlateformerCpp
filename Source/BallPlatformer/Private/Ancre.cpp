#include "Ancre.h"

AAncre::AAncre()
{
	PrimaryActorTick.bCanEverTick = true;

	AnchorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnchorMesh"));
	RootComponent = AnchorMesh;

	AnchorMesh->SetSimulatePhysics(false);
	AnchorMesh->SetMobility(EComponentMobility::Static);
}

void AAncre::BeginPlay()
{
	Super::BeginPlay();
}

void AAncre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}