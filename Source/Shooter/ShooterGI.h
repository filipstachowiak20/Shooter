// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterGI.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterGI : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FText PlayerName;
	
};
