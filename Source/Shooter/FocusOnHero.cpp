// Fill out your copyright notice in the Description page of Project Settings.


#include "FocusOnHero.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UFocusOnHero::TickNode(UBehaviorTreeComponent & OwnerComp,uint8 * NodeMemory,float DeltaSeconds)
{   //rotate to face enemy if one is perceived
    Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);
    auto Enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(PointKey.SelectedKeyName));
    if(!Enemy)
    {   
        OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
        return;
    }
    OwnerComp.GetAIOwner()->SetFocus(Enemy);
}