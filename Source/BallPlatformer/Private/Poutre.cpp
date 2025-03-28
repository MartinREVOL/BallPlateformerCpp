#include "Poutre.h"
#include "Components/StaticMeshComponent.h"

APoutre::APoutre()
{
	PrimaryActorTick.bCanEverTick = true;

	BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));
	RootComponent = BeamMesh;
	BeamMesh->SetSimulatePhysics(true);

	EndA = CreateDefaultSubobject<USceneComponent>(TEXT("EndA"));
	EndA->SetupAttachment(BeamMesh);
	EndA->SetRelativeLocation(FVector(-50.f, 0.f, 0.f));

	EndB = CreateDefaultSubobject<USceneComponent>(TEXT("EndB"));
	EndB->SetupAttachment(BeamMesh);
	EndB->SetRelativeLocation(FVector(50.f, 0.f, 0.f));

	ConstraintA = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintA"));
	ConstraintA->SetupAttachment(BeamMesh);

	ConstraintB = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ConstraintB"));
	ConstraintB->SetupAttachment(BeamMesh);
}

void APoutre::BeginPlay()
{
	Super::BeginPlay();
	SetupConstraints();

	if (BeamBaseMaterial)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(BeamBaseMaterial, this);
		if (DynMaterial)
		{
			BeamMesh->SetMaterial(0, DynMaterial);
		}
	}
}

void APoutre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BeamMesh) return;

	FVector LinearAcceleration = BeamMesh->GetPhysicsLinearVelocity() / DeltaTime;
	float Mass = BeamMesh->GetMass();
	float Force = LinearAcceleration.Size() * Mass;

	UpdateBeamColor(Force);
	if (Force > MaxForce)
	{
		DetachPoutre();
	}
}

void APoutre::SetupConstraints()
{
	if (!ConnecteurA || !ConnecteurB) return;

	UPrimitiveComponent* CompA = Cast<UPrimitiveComponent>(ConnecteurA->GetRootComponent());
	UPrimitiveComponent* CompB = Cast<UPrimitiveComponent>(ConnecteurB->GetRootComponent());

	if (!CompA || !CompB) return;

	ConstraintA->SetWorldLocation(EndA->GetComponentLocation());
	ConstraintB->SetWorldLocation(EndB->GetComponentLocation());

	ConstraintA->SetConstrainedComponents(CompA, NAME_None, BeamMesh, NAME_None);
	ConstraintB->SetConstrainedComponents(BeamMesh, NAME_None, CompB, NAME_None);

	ConstraintA->SetLinearBreakable(true, 50000000);
	ConstraintB->SetLinearBreakable(true, 50000000);

	for (UPhysicsConstraintComponent* Constraint : { ConstraintA, ConstraintB })
	{
		Constraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 0.f);
		Constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 0.f);
		Constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 0.f);
	}
}

void APoutre::UpdateBeamColor(float Force)
{
	float Ratio = FMath::Clamp(Force / MaxForce, 0.f, 1.f);
	FLinearColor Color = FLinearColor::LerpUsingHSV(FLinearColor::Green, FLinearColor::Red, Ratio);

	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("BaseColor", Color);
	}
}

void APoutre::DetachPoutre()
{
	if (ConstraintA)
	{
		ConstraintA->BreakConstraint();
	}

	if (ConstraintB)
	{
		ConstraintB->BreakConstraint();
	}

	// Optional : fais un log, change la couleur, ou joue un effet
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("BaseColor", FLinearColor::Black);
	}

	ConstraintA->DestroyComponent();
	ConstraintB->DestroyComponent();
}
