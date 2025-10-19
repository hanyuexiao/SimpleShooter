// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "MovieSceneSequenceID.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"



// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	Mesh->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::HitTrace(FHitResult& HitResult, FRotator& ShootDirection)
{
    	FVector Location;
    	GetOwnerController() ->GetPlayerViewPoint(Location,ShootDirection);
    	
    	// TODO LineTrace
    	FVector End = Location + ShootDirection.Vector() * Max_Distance;
    	
    	
    	FCollisionQueryParams Params;
    	Params.AddIgnoredActor(this);
    	
    	if(GetOwner())
    	{
    		UE_LOG(LogTemp, Warning, TEXT("Owner is: %s"), *GetOwner()->GetName());
    	}
    	
    	Params.AddIgnoredActor(GetOwner());
    	
    	return  GetWorld()-> LineTraceSingleByChannel(HitResult,Location,End,ECC_GameTraceChannel1,Params);
}

AController* AGun::GetOwnerController() const
{
			APawn* OwnerPawn = Cast<APawn>(GetOwner());
        	if(OwnerPawn == nullptr)
        	{
        		return nullptr;
        	}
    	
        	return OwnerPawn->GetController();
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,Mesh,TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound,Mesh,TEXT("MuzzleFlashSocket"));

	
	FHitResult HitResult;
	FRotator Rotation;

	if(HitTrace(HitResult,Rotation))
	{
		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),HitFlash,HitResult.Location,ShotDirection.Rotation());
		
		if(HitResult.GetActor()==nullptr) return;
		FPointDamageEvent DamageEvent(Damage,HitResult,ShotDirection,nullptr);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(),ImpactSound,HitResult.Location,Rotation);
		HitResult.GetActor() -> TakeDamage(Damage,DamageEvent,GetOwnerController(),this);
		
	}
}

