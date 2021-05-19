// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGM.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Hero.h"
#include "ShooterGI.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Gun.h"
#include "HeroAIController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/UserWidget.h"

AArenaGM::AArenaGM()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AArenaGM::BeginPlay()
{
    Super::BeginPlay();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), StartPoints);
    
    MaxPlayers = FMath::Clamp(MaxPlayers,1,StartPoints.Num()-1);
    SpawnPlayers();
    FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle,this,&AArenaGM::TimeIsUp, TimeLimit, false);
    GameOverTime = FDateTime::Now()+FTimespan(0,0,TimeLimit);
    SetActorTickEnabled(true);

}

void AArenaGM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    TimeRemaining = GameOverTime - FDateTime::Now();
    UE_LOG(LogTemp, Warning, TEXT("sec %d"),TimeRemaining.GetSeconds())
    
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
    Hero->Gun->Destroy();
    if(Cast<AAIController>(Hero->Controller))
    {
        Hero->Health = Hero->MaxHealthAI;
        auto HeroAIController = Cast<AHeroAIController>(Hero->Controller);
        HeroAIController->BrainComponent->StopLogic(FString("respawn"));
        HeroAIController->IsGunFound = false;
        HeroAIController->IsRunningTowardsGun = false;
        HeroAIController->LookForGun();
    }
    else
    {
    Hero->Health = Hero->MaxHealth;
    }

    Scores[ShooterID].Score+=1;
    if(Scores[ShooterID]>HighestScore)
    {
        HighestScore = Scores[ShooterID];
        if(HighestScore.Score >= FragLimit)
        {
            TimeIsUp();
        }
    }
    Hero->IsDead = false;
}

void AArenaGM::RespawnGun()
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys){return;}
	
	FVector Result;
	bool bSuccess = NavSys->K2_GetRandomReachablePointInRadius(GetWorld(), FVector(0,0,0), Result, 20000);
    Result.Z = 120.f;
    GetWorld()->SpawnActor<AGun>(GunClass, Result, FRotator(0,0,0));

}

void AArenaGM::TimeIsUp()
{
    GameOverWidget = CreateWidget<UUserWidget>(GetGameInstance(), GameOverWidgetClass); 
    GameOverWidget->AddToViewport(0);
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}
