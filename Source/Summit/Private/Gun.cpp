// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Camera/CameraComponent.h"
#include "HealthComponent.h"
#include "MainCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	SecondHandPin = CreateDefaultSubobject<USceneComponent>(TEXT("SecondHandPin"));
	SecondHandPin->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	ShotCooldownTime = 1.0f / FireRate;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::Shoot(UCameraComponent* Camera)
{
	float shotTime = UGameplayStatics::GetTimeSeconds(this);
	if (shotTime - LastShotTime < ShotCooldownTime)
	{
		return;
	}

	FHitResult Hit;

	FVector TraceBegin = Camera->GetComponentLocation();
	FVector TraceEnd = Camera->GetComponentLocation() + Camera->GetForwardVector() * 10000.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceBegin, TraceEnd, TraceChannelProperty, QueryParams);

	if (!Hit.bBlockingHit || !IsValid(Hit.GetActor()))
	{
		DrawDebugLine(GetWorld(), TraceBegin, TraceEnd, FColor::Red, false, 5.1f, 0, 1.0f);
		UE_LOG(LogTemp, Log, TEXT("No Actors Hit"));
		return;
	}

	if (AMainCharacter* other = Cast<AMainCharacter>(Hit.GetActor()))
	{
		UE_LOG(LogTemp, Log, TEXT("Trace hit player: %s"), *Hit.GetActor()->GetName());
		DrawDebugLine(GetWorld(), TraceBegin, Hit.ImpactPoint, FColor::Blue, false, 5.1f, 0, 1.0f);
		other->HealthComponent->Server_UpdateHealth(-DamageAmount);
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceBegin, Hit.ImpactPoint, FColor::Red, false, 5.1f, 0, 1.0f);
	}

	LastShotTime = shotTime;
}

