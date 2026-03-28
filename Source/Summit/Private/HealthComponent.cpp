// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UHealthComponent::Server_UpdateHealth_Implementation(float deltaHealth)
{
	Health += deltaHealth;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	Multicast_NotifyHealthChanged(Health);

	if (Health == 0.0f)
	{
		Multicast_NotifyHealthDepleted();
	}
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void UHealthComponent::Multicast_NotifyHealthChanged_Implementation(float newHealth)
{
	OnHealthChanged.Broadcast(newHealth, MaxHealth);
}

void UHealthComponent::Multicast_NotifyHealthDepleted_Implementation()
{
	OnHealthDepleted.Broadcast();
}

