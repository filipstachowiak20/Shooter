// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootHero.h"
#include "AIController.h"
#include "Hero.h"
#include "BehaviorTree/BlackboardComponent.h"

void UShootHero::TickNode(UBehaviorTreeComponent & OwnerComp,uint8 * NodeMemory,float DeltaSeconds)
{   //if Enemy Key is set -> try to shoot at enemy
    Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);
    auto Enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(EnemyKey.SelectedKeyName));
    if(!Enemy){return;}
    Cast<AHero>(OwnerComp.GetAIOwner()->GetPawn())->FireAI(Enemy->GetActorLocation());
}