// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FocusOnHero.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UFocusOnHero : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent & OwnerComp,uint8 * NodeMemory,float DeltaSeconds); 
	UPROPERTY(EditAnywhere)
	struct FBlackboardKeySelector PointKey;
	
};
