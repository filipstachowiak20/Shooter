// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

class UCameraComponent;
class AProjectile;
class AGun;
class AArenaGM;

UCLASS()
class SHOOTER_API AHero : public ACharacter
{
	GENERATED_BODY()


	UPROPERTY(VisibleDefaultsOnly)
	UCameraComponent* Camera;

public:
	UPROPERTY(VisibleDefaultsOnly)
	USkeletalMeshComponent* Hands;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;
	AGun* Gun;
	UPROPERTY()
	AArenaGM* ArenaGMRef;

public:
	// Sets default values for this character's properties
	AHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Fire();
	void FireAI(FVector CurrentEnemyLocation);
	void SpawnProjectile();
	void SpawnProjectileAI();
	void HeroTakeDamage(int32 Damage, int32 ShooterID);
	void BulletTime();
	void StopBulletTime();
	void RegenerateBulletTime();
private:
	int32 ID;
public:
	void SetID(int32 IDtoSet){ID = IDtoSet;}
	int32 GetID(){return ID;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealthAI = 200;
	UPROPERTY(BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere)
	int32 ShotsPerRound = 3;
	UPROPERTY(EditAnywhere)
	float ShotTime = 0.5;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float ReloadTime = 1.5;
	UPROPERTY(BlueprintReadOnly)
	float TimeUntilReload = 0;
	UPROPERTY(EditAnywhere)
	float GunDeviation = 1.5;
	float LastTimeFired = 0;
	UPROPERTY(EditAnywhere)
	float BulletTimeForce = 0.3;
	UPROPERTY(EditAnywhere)
	float BulletTimeDuration = 10;
	UPROPERTY(EditAnywhere)
	float BulletTimeRegeneration = 30;

	FVector EnemyLocation;
	bool IsDead = false;
	bool CanUseBulletTime = true;



};
