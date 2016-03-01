// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "UBTDecorator_HasLoSTo.generated.h"

/**
 * 
 */
UCLASS()
class FPSRTSPROTOTYPE_API UUBTDecorator_HasLoSTo : public UBTDecorator
{
	GENERATED_BODY()

	UUBTDecorator_HasLoSTo(const FObjectInitializer& ObjectInitializer);
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
protected:

	UPROPERTY(EditAnywhere, Category = Condition)
	struct FBlackboardKeySelector EnemyKey;

private:
	bool LOSTrace(AActor* InActor, AActor* InEnemyActor, const FVector& EndLocation) const;
	
};
