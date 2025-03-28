#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ancre.generated.h"

UCLASS()
class BALLPLATFORMER_API AAncre : public AActor
{
	GENERATED_BODY()

public:
	AAncre();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AnchorMesh;
};