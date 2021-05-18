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
	SightConfig->SightRadius = 2000;
	SightConfig->LoseSightRadius = (3000);
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
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
{  
    TArray<FHitResult> HitResultsGuns;
    FCollisionObjectQueryParams QueryParams =FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel3);
    if(!ControlledHero){return;}

    GetWorld()->SweepMultiByObjectType(HitResultsGuns, ControlledHero->GetActorLocation(),ControlledHero->GetActorLocation(),FQuat(),
    QueryParams,FCollisionShape::MakeSphere(1000));
    UE_LOG(LogTemp, Warning, TEXT("%d"),HitResultsGuns.Num())
    if(HitResultsGuns.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("found Gun"))
        MoveToActor(HitResultsGuns[0].GetActor());
        IsGunFound = true;
        IsRunningTowardsGun = true;
        return;
    }

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
        UE_LOG(LogTemp, Warning, TEXT("complete, run behavior"))
        IsRunningTowardsGun = false;
        return;
    }
    if(!IsGunFound)
    {
        LookForGun();
    }
}

void AHeroAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if(Stimulus.WasSuccessfullySensed())
    {
        if(Cast<AHero>(Actor) && IsGunFound && !IsRunningTowardsGun)
        {
        BlackboardComp->SetValueAsObject("Enemy", Actor);
        }
    }
    else if(BlackboardComp)
    {
        BlackboardComp->ClearValue("Enemy");
    }
}