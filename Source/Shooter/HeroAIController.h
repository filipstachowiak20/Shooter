// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "HeroAIController.generated.h"

class AHero;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;
class UBlackboardComponent;


UCLASS()
class SHOOTER_API AHeroAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* PawnSensor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBlackboardComponent* BlackboardComp;
	UPROPERTY(EditAnywhere)
	UBehaviorTree* HeroAIBT;


public:
	AHeroAIController();
	AHero* ControlledHero;

	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result) override; 
	UFUNCTION()
 	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void LookForGun();

	bool IsGunFound = false;
	bool IsRunningTowardsGun = false;
	
};
