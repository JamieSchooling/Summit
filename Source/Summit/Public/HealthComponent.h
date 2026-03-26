// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUMMIT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnHealthDepletedSignature OnHealthDepleted;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	float GetHealth() const;

	float GetMaxHealth() const;

	void UpdateHealth(float deltaHealth);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float MaxHealth = 100.0f;

	UFUNCTION(NetMulticast, Reliable)
	void NotifyHealthDepletedRPC();

	UFUNCTION(NetMulticast, Reliable)
	void NotifyHealthChangedRPC(float newHealth);

		
};
