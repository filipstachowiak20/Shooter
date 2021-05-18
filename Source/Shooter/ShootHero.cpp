// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootHero.h"
#include "AIController.h"
#include "Hero.h"

void UShootHero::TickNode(UBehaviorTreeComponent & OwnerComp,uint8 * NodeMemory,float DeltaSeconds)
{
    Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

    Cast<AHero>(OwnerComp.GetAIOwner()->GetPawn())->Fire();
}