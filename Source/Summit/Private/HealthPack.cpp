// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPack.h"
#include "MainCharacter.h"
#include "HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include <Net/UnrealNetwork.h>

// Sets default values
AHealthPack::AHealthPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AHealthPack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPack::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (AMainCharacter* player = Cast<AMainCharacter>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Entered Health Pack")));

		PlayerToHeal = player;
		HealRPC();
	}
}

void AHealthPack::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHealthPack, PlayerToHeal);
}

void AHealthPack::HealRPC_Implementation()
{
	if (PlayerToHeal == nullptr)
	{
		return;
	}

	float time = UGameplayStatics::GetTimeSeconds(this);

	if (time - LastHealTime < CooldownTime)
	{
		return;
	}

	PlayerToHeal->HealthComponent->UpdateHealthRPC(HealAmount);

	LastHealTime = time;
	PlayerToHeal = nullptr;
}

