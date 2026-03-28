// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SUMMIT_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    // Replicated counters
    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 Kills;

    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 Deaths;

    // Functions to increment safely
    void AddKill();
    void AddDeath();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
