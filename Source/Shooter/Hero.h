// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"

class UCameraComponent;

UCLASS()
class SHOOTER_API AHero : public ACharacter
{
	GENERATED_BODY()


	UPROPERTY(VisibleDefaultsOnly)
	UCameraComponent* Camera;

public:
	UPROPERTY(VisibleDefaultsOnly)
	USkeletalMeshComponent* Hands;
	AActor* Gun;
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

};
