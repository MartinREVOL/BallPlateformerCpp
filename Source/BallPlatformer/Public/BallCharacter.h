#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "StickyObject.h"
#include "EnhancedInputSubsystems.h"
#include "BallCharacter.generated.h"

UCLASS()
class BALLPLATFORMER_API ABallCharacter : public APawn
{
	GENERATED_BODY()

public:
	ABallCharacter();

protected:
	virtual void BeginPlay() override;

private:
	bool bIsOnGround = false;
	bool bGravityInverted = false;
	FTimerHandle WindTimerHandle;
	FTimerHandle WindChangeTimerHandle;
	FVector WindDirection;
	float WindForce;
	bool bIsWindActive = false;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SomeCallbackFunc(const FInputActionValue& Value);
	float GetCurrentSpeed() const;
	bool IsOnGround() const;
	void ToggleWind();
	void StopWind();
	void ChangeWindDirection();

	// 📷 Composants pour la caméra
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	UCameraComponent* Camera;

	// 🎮 Gestion de la caméra
	void LookRightLeft(const FInputActionValue& Value);
	void LookUpDown(const FInputActionValue& Value);
	void MoveBall(const FInputActionValue& Value);

	void ExpelStickyObjects();

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* GravityToggleAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* LookActionX;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* LookActionY;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ExpelAction;


	UPROPERTY(EditDefaultsOnly)
	float JumpCount;

	UPROPERTY(EditDefaultsOnly)
	float JumpImpulse;

	UPROPERTY(EditDefaultsOnly)
	float MaxJumpCount;

	UPROPERTY(EditDefaultsOnly)
	float MovementForce;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MaxSpeed = 1200.0f;

	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
