#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "DestroyableActor.generated.h"

UCLASS()
class BALLPLATFORMER_API ADestroyableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADestroyableActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Composant mesh de l'objet
	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComponent;

	// Détecte une collision avec un StickyObject (par impact)
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Détecte une collision avec un StickyObject (par Overlap)
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
