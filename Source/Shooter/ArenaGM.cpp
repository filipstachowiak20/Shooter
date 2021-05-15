// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGM.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Hero.h"
#include "ShooterGI.h"
#include "AIController.h"



void AArenaGM::BeginPlay()
{
    Super::BeginPlay();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPoints);
    
    MaxPlayers = FMath::Clamp(MaxPlayers,1,StartPoints.Num()-1);
    SpawnPlayers();

}

void AArenaGM::SpawnPlayers()
{
    auto SpawnPoints = StartPoints;
    int32 NumOfSpawnPoints = SpawnPoints.Num();

    auto Hero = Cast<AHero>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    auto CurrentSpawnPoint = SpawnPoints[FMath::RandRange(0,NumOfSpawnPoints-1)];
    Hero->SetActorTransform(CurrentSpawnPoint->GetActorTransform());

    SpawnPoints.Remove(CurrentSpawnPoint);

    FString Name = Cast<UShooterGI>(GetGameInstance())->PlayerName.ToString();
    Scores.Add(FPlayerInfo(Name));
    Hero->SetID(Scores.Num()-1);

    HighestScore = Scores[0];

    for(int32 index = 1; index < MaxPlayers - 1; index++)
    {
        CurrentSpawnPoint = SpawnPoints[FMath::RandRange(0,NumOfSpawnPoints-1-index)];
        auto HeroAI = GetWorld()->SpawnActor<AHero>(HeroClass, CurrentSpawnPoint->GetActorTransform());
        FString NameAI = FString("Player ");
        NameAI.AppendInt(index);
        Scores.Add(FPlayerInfo(NameAI));
        HeroAI->SetID(Scores.Num()-1);
        SpawnPoints.Remove(CurrentSpawnPoint);
    }

}

void AArenaGM::RespawnHero(AHero* Hero,int32 ShooterID)
{
    int32 index = FMath::RandRange(0,StartPoints.Num()-1);
    Hero->SetActorTransform(StartPoints[index]->GetActorTransform());
    if(Cast<AAIController>(Hero->Controller))
    {
        Hero->Health = Hero->MaxHealthAI;
    }
    else
    {
    Hero->Health = Hero->MaxHealth;
    }

    Scores[ShooterID].Score+=1;
    if(Scores[ShooterID]>HighestScore)
    {
        HighestScore = Scores[ShooterID];
    }
}

