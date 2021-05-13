// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Hero.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Gun);
	Capsule->SetupAttachment(Gun);
	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));

	Capsule->OnComponentBeginOverlap.AddDynamic( this, &AGun::BeginOverlap );
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGun::BeginOverlap(UPrimitiveComponent* OverlappedComponent, 
                      AActor* OtherActor, 
                      UPrimitiveComponent* OtherComp, 
                      int32 OtherBodyIndex, 
                      bool bFromSweep, 
                      const FHitResult &SweepResult )
{
	auto Hero = Cast<AHero>(OtherActor);
	if(Hero && !Hero->Gun)
	{

		RotatingMovementComp->DestroyComponent();
		Capsule->DestroyComponent();
		Hero->Gun = this;
		if(Cast<APlayerController>(Hero->Controller))
		{AttachToComponent(Hero->Hands, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GripPoint"));}
		else
		{AttachToComponent(Hero->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GripPoint"));}
	}
}




