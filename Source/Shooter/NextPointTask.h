// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NextPointTask.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UNextPointTask : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp,uint8 * NodeMemory) override; 

protected:
	UPROPERTY(EditAnywhere)
	struct FBlackboardKeySelector PointKey;
	
};
