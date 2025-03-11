#include "DestroyableActor.h"
#include "StickyObject.h"
#include "Components/StaticMeshComponent.h"

ADestroyableActor::ADestroyableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Création du mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Active la physique et les collisions
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetNotifyRigidBodyCollision(true);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// Ajoute l'événement de collision par impact
	MeshComponent->OnComponentHit.AddDynamic(this, &ADestroyableActor::OnHit);

	// Ajoute l'événement de collision Overlap
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestroyableActor::OnOverlap);

	// Permet à l'objet de détecter les "overlaps"
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ADestroyableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADestroyableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestroyableActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AStickyObject* Sticky = Cast<AStickyObject>(OtherActor);
	if (Sticky)
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyableActor DETECTE StickyObject (Hit), destruction !"));
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyableActor a été touché, mais ce n'est pas un StickyObject."));
	}
}

void ADestroyableActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AStickyObject* Sticky = Cast<AStickyObject>(OtherActor);
	if (Sticky)
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyableActor DETECTE StickyObject (Overlap), destruction !"));
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyableActor a été chevauché, mais ce n'est pas un StickyObject."));
	}
}
