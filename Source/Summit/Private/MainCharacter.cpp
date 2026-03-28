// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "Gun.h"
#include "HealthComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include <TPSGameMode.h>

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	// Create a Spring Arm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	//Spring Arm Component is attached to Mesh
	SpringArmComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Create a Third person camera component.
	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));

	//Attach CameraComponent as a child of Spring Arm
	TPSCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	// Create a health component.
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CharacterMovement = GetCharacterMovement();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMainCharacter::BeginDestroy()
{
	Super::BeginDestroy();

	//Gun->Destroy();
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// TODO: Use PostInitializeComponents, then if doesn't work:
	// Try adding delay to see if that fixes standalone

	if (SpringArmComponent)
	{
		//Set Location and Rotation
		SpringArmComponent->SetRelativeLocation(FVector(0.0f, CameraOffset.Y, CameraOffset.Z));
		SpringArmComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

		// Set How far away from character
		SpringArmComponent->TargetArmLength = CameraOffset.X;

		// Set camera lag behaviour
		SpringArmComponent->bEnableCameraLag = true;
		SpringArmComponent->bUsePawnControlRotation = true;
		SpringArmComponent->CameraLagSpeed = CameraLagSpeed;
	}

	if (TPSCameraComponent)
	{
		// Camera pawn rotation must be enabled to allow player to move camera
		TPSCameraComponent->bUsePawnControlRotation = true;
	}

	if (GunClass) 
	{
		//Gun Code
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		Gun->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
		Gun->SetOwner(this);
	}

	if (HealthComponent)
	{
		HealthComponent->OnHealthDepleted.AddDynamic(this, &AMainCharacter::Server_HandleDeath);
	}
}

bool AMainCharacter::IsJumping()
{
	return bIsJumping;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CharacterMovement->bUseControllerDesiredRotation = false;
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jumping);
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AMainCharacter::Shoot);
	}

}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		if (MovementVector.SquaredLength() > 0)
		{
			CharacterMovement->bUseControllerDesiredRotation = true;
		}
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{	
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMainCharacter::Jumping()
{
	Jump();
	bIsJumping = true;
}

void AMainCharacter::Shoot()
{
	Server_Shoot();
}

void AMainCharacter::Server_Shoot_Implementation()
{
	Gun->Shoot_ServerFunc(TPSCameraComponent);
}

void AMainCharacter::Server_HandleDeath_Implementation()
{	
	GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Handling Death")));

	GetWorld()->GetTimerManager().SetTimer(
		RespawnTimerHandle,
		this,
		&AMainCharacter::Respawn_ServerFunc,
		RespawnDelay,
		false
	);
}

void AMainCharacter::Respawn_ServerFunc()
{
	check(HasAuthority())

	GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Attempting Respawn")));
	if (AController* c = GetController())
	{
		c->UnPossess();
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Got Controller for Respawn")));
		// Let the GameMode spawn a new pawn
		if (ATPSGameMode* GM = GetWorld()->GetAuthGameMode<ATPSGameMode>())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Found Game Mode")));
			GM->Server_Respawn(c);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Couldn't get Controller for Respawn")));
	}

	Destroy();
}

void AMainCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	bIsJumping = false;
}

void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainCharacter, bIsJumping);
}


