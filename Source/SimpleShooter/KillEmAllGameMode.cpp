// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled ->GetController());
	if(PlayerController != nullptr)
	{
		EndGame(false);
	}
	
	//For loop over ShooterAi in World
	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		//Is not dead?
		if(!AIController->IsDead())
		{
			//return
			return;
		}
	}
	EndGame(true);
	//End game
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinn)
{
	for(AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinn;
		Controller ->GameHasEnded(Controller->GetPawn(),bIsWinner);
	}
}
