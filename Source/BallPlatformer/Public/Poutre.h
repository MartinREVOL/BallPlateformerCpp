#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Poutre.generated.h"

UCLASS()
class BALLPLATFORMER_API APoutre : public AActor
{
	GENERATED_BODY()

public:
	APoutre();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetupConstraints();
	void UpdateBeamColor(float Force);
	void DetachPoutre();

	UPROPERTY(EditAnywhere, Category = "Stress")
	float MaxForce = 10000000.f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BeamMesh;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* ConstraintA;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* ConstraintB;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* EndA;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* EndB;

	UPROPERTY(EditInstanceOnly, Category = "Bridge")
	AActor* ConnecteurA;

	UPROPERTY(EditInstanceOnly, Category = "Bridge")
	AActor* ConnecteurB;

	UPROPERTY(EditAnywhere, Category = "Material")
	UMaterialInterface* BeamBaseMaterial;

private:
	UMaterialInstanceDynamic* DynMaterial;
};