// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationifSeen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_PlayerLocationifSeen::UBTService_PlayerLocationifSeen()
{
	NodeName = "Update Player Location";
}

void UBTService_PlayerLocationifSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);

	if(playerPawn == nullptr)
	{
		return;
	}

	if(OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}
	
	if(OwnerComp.GetAIOwner()->LineOfSightTo(playerPawn))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(),playerPawn);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
