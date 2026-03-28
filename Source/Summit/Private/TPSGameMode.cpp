// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerStart.h>

void ATPSGameMode::Server_Respawn_Implementation(AController* PlayerController)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Entered Respawn")));

    GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange, FString::Printf(TEXT("Valid respawn call")));
    FVector SpawnLocation = ChoosePlayerStart(PlayerController)->GetActorLocation();
    FRotator SpawnRotation = ChoosePlayerStart(PlayerController)->GetActorRotation();

    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation);
    PlayerController->Possess(NewPawn);
}

AActor* ATPSGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, PlayerStarts.Num() - 1);
        return PlayerStarts[Index];
    }

    return Super::ChoosePlayerStart_Implementation(Player);
}