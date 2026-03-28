// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "MainCharacter.generated.h"

class AGun;
class UHealthComponent;

UCLASS()
class SUMMIT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	/** Mapping Context file */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* ShootAction;

	// TPS camera
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* TPSCameraComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector CameraOffset{ 350.0f, 30.0f, 100.0f };

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraLagSpeed = 20.0f;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly) // Not editable in run time
	TSubclassOf<AGun> GunClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float RespawnDelay = 3.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void PostInitializeComponents() override;

public:	
	UFUNCTION(BlueprintCallable)
	bool IsJumping();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Jumping input */
	void Jumping();

	/** Called for Shoot input */
	void Shoot();

	/** Performs shoot on the server */
	UFUNCTION(Server, Reliable)
	void Server_Shoot();

	UFUNCTION(Server, Reliable)
	void Server_HandleDeath(AActor* Killer);
	void Respawn_ServerFunc();

	virtual void Landed(const FHitResult& Hit) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;
	UPROPERTY(Replicated)
	bool bIsJumping = false;
	FTimerHandle RespawnTimerHandle;
};
