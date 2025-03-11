// Fill out your copyright notice in the Description page of Project Settings.

#include "BallCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Slate/SGameLayerManager.h"

// Sets default values
ABallCharacter::ABallCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Création de la sphère (Balle)
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->BodyInstance.SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->BodyInstance.MassScale = 1.0f;
	SphereComponent->OnComponentHit.AddDynamic(this, &ABallCharacter::OnSphereHit);

	// Ajout du SpringArm (permet à la caméra de suivre la balle avec un décalage)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;  // Distance de la caméra par rapport à la balle
	SpringArm->bUsePawnControlRotation = true; // Permet de tourner avec la souris

	// Ajout de la caméra
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // La caméra ne suit que le SpringArm
	Camera->SetAutoActivate(true); // ✅ Assure que la caméra est activée par défaut

	// Désactiver la rotation du Pawn (on utilise le SpringArm à la place)
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ABallCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Vérifier que le PlayerController possède bien cette Pawn comme caméra active
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->SetViewTarget(this); // ✅ Définit ce personnage comme la caméra active
	}

	// Ajout du contexte d'entrée pour Enhanced Input
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (!InputMapping.IsNull())
			{
				PlayerSubsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}
}


void ABallCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!SphereComponent->IsSimulatingPhysics())
	{
		bIsOnGround = false;
	}

	if (bIsOnGround)
	{
		JumpCount = 0;
	}

	// Appliquer la force du vent en continu
	if (bIsWindActive)
	{
		SphereComponent->AddForce(WindDirection * WindForce * DeltaTime * 50.0f);
	}

	// Limiter la vitesse de la balle
	FVector Velocity = SphereComponent->GetComponentVelocity();
	float Speed = Velocity.Size();

	if (Speed > MaxSpeed)
	{
		// Réduire la vitesse en normalisant la direction et en appliquant MaxSpeed
		FVector ClampedVelocity = Velocity.GetSafeNormal() * MaxSpeed;
		SphereComponent->SetPhysicsLinearVelocity(ClampedVelocity);
	}
}




void ABallCharacter::SomeCallbackFunc(const FInputActionValue& Value)
{
	if (Value.GetValueType() == EInputActionValueType::Boolean) // Saut
	{
		bool bJumpPressed = Value.Get<bool>();
		if ((bJumpPressed && IsOnGround()) || JumpCount < 2) // Vérifier si la balle est sur le sol
		{
			FVector JumpImpulseVector = FVector(0.0f, 0.0f, JumpImpulse);
			SphereComponent->AddImpulse(JumpImpulseVector);
			bIsOnGround = false; // La balle quitte le sol après un saut
			JumpCount++;
		}
	}
}

float ABallCharacter::GetCurrentSpeed() const
{
	if (SphereComponent)
	{
		// Récupérer la vitesse actuelle (magnitude du vecteur vitesse)
		return SphereComponent->GetComponentVelocity().Size();
	}

	return 0.0f;
}

bool ABallCharacter::IsOnGround() const
{
	return bIsOnGround;
}

void ABallCharacter::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
 {
	// Vérifiez si la collision est avec une surface (sol) en fonction de la normale
	if (FMath::Abs(Hit.Normal.Z) > 0.7f) // Si la normale est principalement verticale
	{
		bIsOnGround = true;
	}
}

// Called to bind functionality to input
void ABallCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABallCharacter::MoveBall);
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABallCharacter::SomeCallbackFunc);
	Input->BindAction(GravityToggleAction, ETriggerEvent::Triggered, this, &ABallCharacter::ToggleWind);

	Input->BindAction(LookActionX, ETriggerEvent::Triggered, this, &ABallCharacter::LookRightLeft);
	Input->BindAction(LookActionY, ETriggerEvent::Triggered, this, &ABallCharacter::LookUpDown);

	Input->BindAction(ExpelAction, ETriggerEvent::Triggered, this, &ABallCharacter::ExpelStickyObjects);
}

void ABallCharacter::ToggleWind()
{
	// Générer une force initiale aléatoire
	WindForce = FMath::RandRange(500.0f, 1000.0f);

	// Activer le vent
	bIsWindActive = true;

	// Changer la direction immédiatement
	ChangeWindDirection();

	UE_LOG(LogTemp, Warning, TEXT("Vent activé : Force %.1f"), WindForce);

	// Démarrer un timer pour changer la direction toutes les secondes
	GetWorldTimerManager().SetTimer(WindChangeTimerHandle, this, &ABallCharacter::ChangeWindDirection, 1.0f, true);

	// Démarrer un timer pour désactiver le vent après 10 secondes
	GetWorldTimerManager().SetTimer(WindTimerHandle, this, &ABallCharacter::StopWind, 10.0f, false);
}

void ABallCharacter::ChangeWindDirection()
{
	// Générer une nouvelle direction aléatoire (horizontale)
	WindDirection = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.0f).GetSafeNormal();

	UE_LOG(LogTemp, Warning, TEXT("Nouvelle direction du vent : (%s)"), *WindDirection.ToString());
}


void ABallCharacter::StopWind()
{
	bIsWindActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Vent désactivé."));
}


// Faire tourner uniquement la caméra
void ABallCharacter::LookRightLeft(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	if (AController* PlayerController = GetController())
	{
		FRotator NewRotation = PlayerController->GetControlRotation();
		NewRotation.Yaw += AxisValue;
		PlayerController->SetControlRotation(NewRotation);
	}
}

void ABallCharacter::LookUpDown(const FInputActionValue& Value)
{
	float AxisValue = Value.Get<float>();
	if (AController* PlayerController = GetController())
	{
		FRotator NewRotation = PlayerController->GetControlRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + AxisValue, -80.0f, 80.0f); // Limite l'angle vertical
		PlayerController->SetControlRotation(NewRotation);
	}
}

// Déplacement de la balle basé sur la direction de la caméra
void ABallCharacter::MoveBall(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (SphereComponent)
	{
		FRotator CameraYawRotation = GetControlRotation();
		CameraYawRotation.Pitch = 0;
		CameraYawRotation.Roll = 0;

		FVector ForwardDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::Y);

		FVector Force = (ForwardDirection * MovementVector.X + RightDirection * MovementVector.Y) * MovementForce;
		SphereComponent->AddForce(Force);
	}
}

void ABallCharacter::ExpelStickyObjects()
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors); // Récupère tous les objets attachés à la balle

	for (AActor* Actor : AttachedActors)
	{
		AStickyObject* Sticky = Cast<AStickyObject>(Actor);
		if (Sticky)
		{
			// Détacher l'objet de la balle
			Sticky->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			// Réactiver la physique et les collisions
			UStaticMeshComponent* StickyMesh = Sticky->FindComponentByClass<UStaticMeshComponent>();
			if (StickyMesh)
			{
				StickyMesh->SetSimulatePhysics(true);
				StickyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}

			// Expulser l'objet avec une force aléatoire
			FVector ExpelDirection = (Sticky->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			ExpelDirection += FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0.5f); // Petite variation pour éviter un mouvement trop uniforme
			StickyMesh->AddImpulse(ExpelDirection * 5000.0f * 5.0f); // Appliquer la force d'expulsion
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Tous les objets sticky ont été expulsés !"));
}

