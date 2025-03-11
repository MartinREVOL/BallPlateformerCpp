#include "StickyObject.h"
#include "Components/StaticMeshComponent.h"
#include "BallCharacter.h"

AStickyObject::AStickyObject()
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

	// Ajoute l'événement de collision
	MeshComponent->OnComponentHit.AddDynamic(this, &AStickyObject::OnHit);
}

// Appelé au début du jeu
void AStickyObject::BeginPlay()
{
	Super::BeginPlay();
}

// Tick (facultatif pour animer l'objet)
void AStickyObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Détection de la collision avec la balle
void AStickyObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABallCharacter* Ball = Cast<ABallCharacter>(OtherActor);
	if (Ball)
	{
		AttachToBall(Ball);
	}

	ADestroyableActor* Wall = Cast<ADestroyableActor>(OtherActor);
	if (Wall)
	{
		OtherActor->Destroy();
	}
}

// Fonction qui attache l'objet à la balle
void AStickyObject::AttachToBall(AActor* Ball)
{
	if (!Ball) return;

	// Désactive la physique pour que l'objet suive la balle
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Attache l'objet à la balle
	AttachToActor(Ball, FAttachmentTransformRules::KeepWorldTransform);
	
	UE_LOG(LogTemp, Warning, TEXT("Objet attaché à la balle !"));
}

void AStickyObject::ReactivateCollisions()
{
	// Réactiver les collisions et la physique
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
