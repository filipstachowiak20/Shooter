// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Gun.h"
#include "ArenaGM.h"
#include "AIController.h"
#include "HeroAIController.h"
#include "Kismet/KismetMathLibrary.h"

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
	//set health points for player/AI 
	ArenaGMRef = Cast<AArenaGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if(Cast<AHeroAIController>(Controller))
	{
		Health = MaxHealthAI;
		Cast<AHeroAIController>(Controller)->ControlledHero = this;
		Cast<AHeroAIController>(Controller)->LookForGun();
	}
	else
	{
	Health = MaxHealth;
	}
	
}


void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//manage when can fire gun again
	if(FPlatformTime::Seconds()-LastTimeFired>=ReloadTime)
	{
		TimeUntilReload = 0;
	}
	else
	{
		TimeUntilReload = ReloadTime - FPlatformTime::Seconds()+ LastTimeFired;
	}


}

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHero::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("BulletTime", IE_Pressed, this, &AHero::BulletTime);
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
	if(!Gun){return;}
	if(TimeUntilReload>0){return;}
	TArray<FTimerHandle> handles;
	handles.SetNum(ShotsPerRound);
	for(int32 index = 1; index <=ShotsPerRound ; index++)
	{	//spawn X projectiles in time intervals
		float Duration = ShotTime/ShotsPerRound*index;
		GetWorld()->GetTimerManager().SetTimer(handles[index-1],this,&AHero::SpawnProjectile, Duration, false);
	}
	LastTimeFired = FPlatformTime::Seconds();
}

void AHero::FireAI(FVector CurrentEnemyLocation)
{
	EnemyLocation = CurrentEnemyLocation;
	if(!Gun){return;}
	if(TimeUntilReload>0){return;}
	TArray<FTimerHandle> handles;
	handles.SetNum(ShotsPerRound);
	for(int32 index = 1; index <=ShotsPerRound ; index++)
	{
		float Duration = ShotTime/ShotsPerRound*index;
		GetWorld()->GetTimerManager().SetTimer(handles[index-1],this,&AHero::SpawnProjectileAI, Duration, false);
	}
	LastTimeFired = FPlatformTime::Seconds();	
}

void AHero::HeroTakeDamage(int32 Damage, int32 ShooterID)
{	//reduct health, tell gamemode to respawn if player's dead
	Health = FMath::Clamp(Health-Damage,0,FMath::Max3(MaxHealth,MaxHealth,MaxHealthAI));
	if(Health == 0 && !IsDead)
	{
		IsDead = true;
		if(ArenaGMRef)
		{ArenaGMRef->RespawnHero(this, ShooterID);}
	}
}

void AHero::SpawnProjectile()
{
	if(!Gun || Gun->IsPendingKillPending()){return;}
	FVector MuzzleLocation;
	FRotator MuzzleRotation;
	Gun->GetGunMesh()->GetSocketWorldLocationAndRotation(FName("Muzzle"), MuzzleLocation, MuzzleRotation);
	FRotator SpawnRotation = GetControlRotation()+ FRotator(FMath::RandRange(-1*GunDeviation,GunDeviation));
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, SpawnRotation, SpawnParams);
	if(Projectile)
	{
		Projectile->SetShooterID(GetID());//If projectile kills player, ID will identify which player shot it
	}

	TimeUntilReload += ReloadTime/ShotsPerRound;
}

void AHero::SpawnProjectileAI()
{
	if(!Gun ||Gun->IsPendingKillPending()){return;}
	FVector MuzzleLocation;
	FRotator MuzzleRotation;
	Gun->GetGunMesh()->GetSocketWorldLocationAndRotation(FName("Muzzle"), MuzzleLocation, MuzzleRotation);
	FRotator Direction = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),EnemyLocation);
	FRotator SpawnRotation = Direction + FRotator(FMath::RandRange(-1*GunDeviation,GunDeviation));
	FVector SpawnLocation = MuzzleLocation - GetActorRightVector()*50;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if(Projectile)
	{
		Projectile->SetShooterID(GetID());
	}

	TimeUntilReload += ReloadTime/ShotsPerRound;
}

void AHero::BulletTime()
{
	if(CanUseBulletTime)
	{	//set time dilation for world and player
		GetWorld()->GetWorldSettings()->SetTimeDilation(BulletTimeForce);
		CustomTimeDilation = 1/BulletTimeForce;
		//reset time dilation after set time
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle,this,&AHero::StopBulletTime, BulletTimeDuration*BulletTimeForce, false);
		CanUseBulletTime = false;
	}
}

void AHero::StopBulletTime()
{//reverse time dilation
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
	CustomTimeDilation = 1.f;
	FTimerHandle handle;
	//handle when bullet time can be used again
	GetWorld()->GetTimerManager().SetTimer(handle,this,&AHero::RegenerateBulletTime, BulletTimeRegeneration, false);
}

void AHero::RegenerateBulletTime()
{
	CanUseBulletTime = true;
}