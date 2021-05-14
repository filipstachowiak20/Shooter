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
	void HeroTakeDamage(int32 Damage, int32 ShooterID);
private:
	int32 ID;
public:
	void SetID(int32 IDtoSet){ID = IDtoSet;}
	int32 GetID(){return ID;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth = 100;
	UPROPERTY(BlueprintReadWrite)
	int32 Health;


};
