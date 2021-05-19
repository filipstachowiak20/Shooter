// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGM.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Hero.h"
#include "ShooterGI.h"
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
    
    MaxPlayers = FMath::Clamp(MaxPlayers,1,StartPoints.Num()-1); //cant have more players then player starts
    SpawnPlayers();
    FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle,this,&AArenaGM::TimeIsUp, TimeLimit, false); //game is finished after certain time
    GameOverTime = FDateTime::Now()+FTimespan(0,0,TimeLimit);
    SetActorTickEnabled(true);

}

void AArenaGM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
    TimeRemaining = GameOverTime - FDateTime::Now();//update remaining time
}
void AArenaGM::SpawnPlayers()
{
    auto SpawnPoints = StartPoints;
    int32 NumOfSpawnPoints = SpawnPoints.Num();
    //Set player inicial location randomly
    auto Hero = Cast<AHero>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    auto CurrentSpawnPoint = SpawnPoints[FMath::RandRange(0,NumOfSpawnPoints-1)];
    Hero->SetActorTransform(CurrentSpawnPoint->GetActorTransform());

    SpawnPoints.Remove(CurrentSpawnPoint);
    //get player name and set its score as first in the scores array
    FString Name = Cast<UShooterGI>(GetGameInstance())->PlayerName.ToString();
    Scores.Add(FPlayerInfo(Name));
    Hero->SetID(Scores.Num()-1);

    HighestScore = Scores[0];
    //spawn AI players randomly, assign them names
    for(int32 index = 1; index < MaxPlayers - 1; index++)
    {
        CurrentSpawnPoint = SpawnPoints[FMath::RandRange(0,NumOfSpawnPoints-1-index)];
        auto HeroAI = GetWorld()->SpawnActor<AHero>(HeroClass, CurrentSpawnPoint->GetActorTransform());
        FString NameAI = FString("Player");
        NameAI.AppendInt(index);
        Scores.Add(FPlayerInfo(NameAI));
        HeroAI->SetID(Scores.Num()-1);
        SpawnPoints.Remove(CurrentSpawnPoint);
    }

}

void AArenaGM::RespawnHero(AHero* Hero,int32 ShooterID)
{
    if(!Hero->GetController()){return;}

    int32 index = FMath::RandRange(0,StartPoints.Num()-1);
    Hero->TeleportTo(StartPoints[index]->GetActorLocation(),StartPoints[index]->GetActorRotation(),false,false);//ramdomly set location
    AGun* HeroGun = Hero->Gun;
    Hero->Gun = nullptr;
    if(HeroGun){HeroGun->Destroy();}
    if(Cast<AHeroAIController>(Hero->GetController()))
    {   //set default stats for AI
        Hero->Health = Hero->MaxHealthAI;
        auto HeroAIControllerRef = Cast<AHeroAIController>(Hero->GetController());
        HeroAIControllerRef->IsGunFound = false;
        HeroAIControllerRef->IsRunningTowardsGun = false;
        HeroAIControllerRef->LookForGun();
    }
    else
    {//Set default stats for player
    Hero->Health = Hero->MaxHealth;
    }
    Hero->IsDead = false;
    if(ShooterID == -1){return;}
    //update scores
    Scores[ShooterID].Score+=1;
    if(Scores[ShooterID]>HighestScore)
    {
        HighestScore = Scores[ShooterID];
        if(HighestScore.Score >= FragLimit)
        {
            TimeIsUp();//finish game before time if score>frag limit
        }
    }
}

void AArenaGM::RespawnGun()
{   //spawn gun at random location after player picked it up
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys){return;}
	
	FVector Result;
	bool bSuccess = NavSys->K2_GetRandomReachablePointInRadius(GetWorld(), FVector(0,0,0), Result, 50000);
    Result.Z = 120.f;
    GetWorld()->SpawnActor<AGun>(GunClass, Result, FRotator(0,0,0));

}

void AArenaGM::TimeIsUp()
{   //show game over screen, pause the game
    GameOverWidget = CreateWidget<UUserWidget>(GetGameInstance(), GameOverWidgetClass); 
    GameOverWidget->AddToViewport(0);
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}
