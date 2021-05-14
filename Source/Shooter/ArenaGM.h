// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGM.generated.h"

class AHero;

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
	public:
		FPlayerInfo(){}
		FPlayerInfo(FString InputName)
		{
			Name = InputName;
			Score = 0;
		}
    	UPROPERTY(BlueprintReadWrite)
    	FString Name;
		UPROPERTY(BlueprintReadWrite)
    	int32 Score;

		bool operator==(const FPlayerInfo& Info) const {
      	return (Score == Info.Score);}
        bool operator>(const FPlayerInfo& Info) const {
      	return (Score > Info.Score);}
        bool operator<(const FPlayerInfo& Info) const {
      	return (Score < Info.Score);}
};

UCLASS()
class SHOOTER_API AArenaGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	int32 MaxPlayers = 5;
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> StartPoints;
	UPROPERTY(BlueprintReadOnly)
	TArray<FPlayerInfo> Scores; 
	UPROPERTY(BlueprintReadOnly)
	FPlayerInfo HighestScore;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHero> HeroClass;

public:
	void SpawnPlayers();
	void RespawnHero(AHero* Hero,int32 ShooterID);
	
};
