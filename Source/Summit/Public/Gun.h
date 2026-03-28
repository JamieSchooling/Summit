// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class UCameraComponent;

UCLASS()
class SUMMIT_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun(); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Shoot_ServerFunc(UCameraComponent* Camera);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SecondHandPin;

	UPROPERTY(EditAnywhere, Category = "Default")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	UPROPERTY(EditAnywhere)
	float DamageAmount = 10.0f;

	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	float ShotCooldownTime;
	float LastShotTime = 0.0f;
};
