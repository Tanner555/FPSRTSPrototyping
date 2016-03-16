// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "UBTTask_FindPickup.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "RTSControllerCPP.h"
#include "AllyMember.h"

EBTNodeResult::Type UUBTTask_FindPickup::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//TODO:Write a proper find pickup task.
	ARTSControllerCPP* MyController = Cast<ARTSControllerCPP>(OwnerComp.GetAIOwner());
	AAllyMember* MyBot = MyController ? Cast<AAllyMember>(MyController->GetPawn()) : NULL;
	if (MyBot == NULL)
	{
		return EBTNodeResult::Failed;
	}

	//AShooterGameMode* GameMode = MyBot->GetWorld()->GetAuthGameMode<AShooterGameMode>();
	/*if (GameMode == NULL)
	{
		return EBTNodeResult::Failed;
	}*/

	const FVector MyLoc = MyBot->GetActorLocation();
	//AShooterPickup_Ammo* BestPickup = NULL;
	float BestDistSq = MAX_FLT;

	/*for (int32 i = 0; i < GameMode->LevelPickups.Num(); ++i)
	{
		AShooterPickup_Ammo* AmmoPickup = Cast<AShooterPickup_Ammo>(GameMode->LevelPickups[i]);
		if (AmmoPickup && AmmoPickup->IsForWeapon(AShooterWeapon_Instant::StaticClass()) && AmmoPickup->CanBePickedUp(MyBot))
		{
			const float DistSq = (AmmoPickup->GetActorLocation() - MyLoc).SizeSquared();
			if (BestDistSq == -1 || DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPickup = AmmoPickup;
			}
		}
	}*/

	if (/*BestPickup*/false)
	{
		//OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), BestPickup->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

