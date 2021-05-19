// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* Collision;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovement;
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	int32 ShooterID = -1;

public:
	int32 GetShooterID(){return ShooterID;}
	void SetShooterID(int32 IDtoSet){ShooterID = IDtoSet;}

	UPROPERTY(EditAnywhere)
	int32 Damage = 10;
	UPROPERTY(EditAnywhere)
	float Force = 3000;

};
