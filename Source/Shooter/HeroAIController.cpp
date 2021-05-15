// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroAIController.h"
#include "Hero.h"

AHeroAIController::AHeroAIController()
{
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
    if(IsGunFound){return;}
    LookForGun();
}