// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"

void ATPSGameMode::Server_Respawn_Implementation(AController* PlayerController)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Entered Respawn")));

    GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Valid respawn call")));
    FVector SpawnLocation = ChoosePlayerStart(PlayerController)->GetActorLocation();
    FRotator SpawnRotation = ChoosePlayerStart(PlayerController)->GetActorRotation();

    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation);
    PlayerController->Possess(NewPawn);
}
