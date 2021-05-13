// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

// Sets default values
AHero::AHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	Camera->bUsePawnControlRotation = true;

	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Hands->SetupAttachment(Camera);

	Hands->SetOnlyOwnerSee(true);
	Hands->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Hands->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHero::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHero::Fire);

}

void AHero::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AHero::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void AHero::Fire()
{
	if(Gun)
	{

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("nie masz broni"))
	}
}