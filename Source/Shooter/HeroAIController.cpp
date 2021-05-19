// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAIController.h"
#include "Hero.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AHeroAIController::AHeroAIController()
{
    PawnSensor = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PawnSensor"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	PawnSensor->SetDominantSense(SightConfig->GetSenseImplementation());
	PawnSensor->ConfigureSense(*SightConfig);
	SightConfig->SightRadius = 2500;
	SightConfig->LoseSightRadius = (3000);
	SightConfig->PeripheralVisionAngleDegrees = 120.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PawnSensor->ConfigureSense(*SightConfig);
    SetPerceptionComponent(*PawnSensor);

    GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AHeroAIController::OnTargetPerceptionUpdated);
}

void AHeroAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AHeroAIController::LookForGun()
{  //check if there is a gun in radius, if so go pick it up
    TArray<FHitResult> HitResultsGuns;
    FCollisionObjectQueryParams QueryParams =FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel3);
    if(!ControlledHero){return;}
    FVector Loc = ControlledHero->GetActorLocation()+1000*ControlledHero->GetActorForwardVector();
    GetWorld()->SweepMultiByObjectType(HitResultsGuns, ControlledHero->GetActorLocation(),ControlledHero->GetActorLocation(),FQuat(),
    QueryParams,FCollisionShape::MakeSphere(2000));
    if(HitResultsGuns.Num() > 0)
    {
        MoveToActor(HitResultsGuns[0].GetActor());
        IsGunFound = true;
        IsRunningTowardsGun = true;
        return;
    }
    //gun not found, go to another random point
    TArray<FHitResult> HitResultsPoints;
    FCollisionObjectQueryParams QueryParams2 =FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2);

    GetWorld()->SweepMultiByObjectType(HitResultsPoints, ControlledHero->GetActorLocation(),ControlledHero->GetActorLocation(),FQuat(),
    QueryParams2,FCollisionShape::MakeSphere(1000));

    if(HitResultsPoints.Num()<1)
    {return;}

    auto RandomPoint = HitResultsPoints[FMath::RandRange(0,HitResultsPoints.Num()-1)].GetActor();

    MoveToActor(RandomPoint);
    
}

void AHeroAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
    Super::OnMoveCompleted(RequestID, Result);
    if(IsGunFound && IsRunningTowardsGun)
    {
        BlackboardComp->InitializeBlackboard(*HeroAIBT->BlackboardAsset);
        RunBehaviorTree(HeroAIBT);
        IsRunningTowardsGun = false;
        if(!ControlledHero->Gun)
        {//gun was not there anymore when player got there, look for it again
            IsGunFound = false;
            LookForGun();
        }
        return;
    }
    if(!IsGunFound)
    {//gun not found, look for it again
        LookForGun();
    }
}

void AHeroAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if(Stimulus.WasSuccessfullySensed())
    {
        if(BlackboardComp->GetValueAsObject("Enemy"))
        {//stick to first spotted enemy
            return;}
        if(Cast<AHero>(Actor) && IsGunFound && !IsRunningTowardsGun)
        {
        BlackboardComp->SetValueAsObject("Enemy", Actor);
        }
    }
    else if(BlackboardComp  && Actor == Cast<AActor>(BlackboardComp->GetValueAsObject("Enemy")))
    {//allow to stop chasing only if lost sight of primary enemy
        BlackboardComp->ClearValue("Enemy");
    }
}