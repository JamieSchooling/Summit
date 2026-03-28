// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPack.generated.h"

class AMainCharacter;

UCLASS()
class SUMMIT_API AHealthPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPack();

	UPROPERTY(EditAnywhere)
	float HealAmount = 50.0f;

	UPROPERTY(EditAnywhere)
	float CooldownTime = 10.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UFUNCTION(Server, Reliable)
	void Server_Heal();

	float LastHealTime = 0.0f;

	UPROPERTY(Replicated)
	AMainCharacter* PlayerToHeal;
};
