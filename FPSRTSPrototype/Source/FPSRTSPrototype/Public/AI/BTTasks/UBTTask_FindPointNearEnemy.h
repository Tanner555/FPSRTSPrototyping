// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UBTTask_FindPointNearEnemy.generated.h"

/**
 * 
 */
UCLASS()
class FPSRTSPROTOTYPE_API UUBTTask_FindPointNearEnemy : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
