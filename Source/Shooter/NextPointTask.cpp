// Fill out your copyright notice in the Description page of Project Settings.


#include "NextPointTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Hero.h"

EBTNodeResult::Type UNextPointTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp,uint8 * NodeMemory)
{
    auto AIController = OwnerComp.GetAIOwner();
    auto ControlledPawn = AIController->GetPawn();
    auto BlackboardComp = OwnerComp.GetBlackboardComponent();

    TArray<FHitResult> HitResultsPoints;
    FCollisionObjectQueryParams QueryParams =FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2);

    GetWorld()->SweepMultiByObjectType(HitResultsPoints, ControlledPawn->GetActorLocation(),ControlledPawn->GetActorLocation(),FQuat(),
    QueryParams,FCollisionShape::MakeSphere(1000));

    auto RandomPoint = HitResultsPoints[FMath::RandRange(0,HitResultsPoints.Num()-1)].GetActor();

    BlackboardComp->SetValueAsObject(PointKey.SelectedKeyName,RandomPoint);
    return EBTNodeResult::Succeeded;
}