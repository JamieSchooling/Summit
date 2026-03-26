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

void UHealthComponent::UpdateHealth(float deltaHealth)
{
	Health += deltaHealth;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	NotifyHealthChangedRPC(Health);

	if (Health == 0.0f)
	{
		NotifyHealthDepletedRPC();
	}
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void UHealthComponent::NotifyHealthChangedRPC_Implementation(float newHealth)
{
	OnHealthChanged.Broadcast(newHealth, MaxHealth);
}

void UHealthComponent::NotifyHealthDepletedRPC_Implementation()
{
	OnHealthDepleted.Broadcast();
}

