#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyableActor.h"
#include "Components/StaticMeshComponent.h"
#include "StickyObject.generated.h"

UCLASS()
class BALLPLATFORMER_API AStickyObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AStickyObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Composant pour afficher l'objet
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComponent;

	// Détecte la collision avec la balle
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Fonction pour attacher l'objet à la balle
	void AttachToBall(AActor* Ball);

	void ReactivateCollisions();

};
