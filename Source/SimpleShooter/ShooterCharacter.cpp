// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include <Programs/UnrealBuildAccelerator/Core/Public/UbaBase.h>

#include "Gun.h"
#include "SimpleShooterGameModeBash.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Current_Health = Max_Health;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	//TODO Set Gun
	Gun = GetWorld() -> SpawnActor<AGun>(GunClass);
	GetMesh() -> HideBoneByName(TEXT("weapon_r"),PBO_None);
	Gun->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));
	Gun->SetOwner(this);

	//TODO Set Health


}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&AShooterCharacter::MoveForward);

	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookRight"),this,&APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed,this,&ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Shooter"),IE_Pressed,this,&AShooterCharacter::Shoot);
}

void AShooterCharacter::MoveForward (float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::Jump(float AxisValue)
{
	AddMovementInput(GetActorUpVector() * AxisValue);
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}
float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Suffer_Damage = Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	float DamageAplplied = FMath::Min(Suffer_Damage,Current_Health);
	Current_Health -= DamageAplplied;
	UE_LOG(LogTemp,Warning,TEXT("Health is %f"),Current_Health);
	
	if(IsDead())
	{
		ASimpleShooterGameModeBash* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBash>();

		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));

	}
	return Current_Health;
}


bool AShooterCharacter::IsDead() const
{
	if (Current_Health > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

float AShooterCharacter::GetHealthPercent() const
{
	return Current_Health/Max_Health;
}
