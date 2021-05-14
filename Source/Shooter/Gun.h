// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class URotatingMovementComponent;
class UCapsuleComponent;

UCLASS()
class SHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* GunMesh;
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* Capsule;
	UPROPERTY(EditDefaultsOnly)
	URotatingMovementComponent* RotatingMovementComp;
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION( )
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, 
                      AActor* OtherActor, 
                      UPrimitiveComponent* OtherComp, 
                      int32 OtherBodyIndex, 
                      bool bFromSweep, 
                      const FHitResult &SweepResult );

	USkeletalMeshComponent* GetGunMesh();
};
