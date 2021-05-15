// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HeroAIController.generated.h"

class AHero;

UCLASS()
class SHOOTER_API AHeroAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHeroAIController();
	AHero* ControlledHero;

	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result) override; 

	void LookForGun();

	bool IsGunFound = false;
	
};
