#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Connecteur.generated.h"

UCLASS()
class BALLPLATFORMER_API AConnecteur : public AActor
{
	GENERATED_BODY()

public:
	AConnecteur();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ConnectorMesh;
};