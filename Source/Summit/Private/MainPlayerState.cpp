// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerState.h"
#include <Net/UnrealNetwork.h>

void AMainPlayerState::AddKill()
{
    if (HasAuthority())
    {
        ++Kills;
    }
}

void AMainPlayerState::AddDeath()
{
    if (HasAuthority())
    {
        ++Deaths;
    }
}

void AMainPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMainPlayerState, Kills);
    DOREPLIFETIME(AMainPlayerState, Deaths);
}
