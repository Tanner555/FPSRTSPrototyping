// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/ShooterPickup.h"
#include "ShooterPickup_Health.generated.h"

class AllyMember;
/**
 * 
 */
UCLASS()
class FPSRTSPROTOTYPE_API AShooterPickup_Health : public AShooterPickup
{
	GENERATED_BODY()

	AShooterPickup_Health(const FObjectInitializer& ObjectInitializer);
	
	virtual bool CanBePickedUp(class AAllyMember* TestPawn) const override;

protected:

	/** how much health does it give? */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		float Health;

	/** give pickup */
	virtual void GivePickupTo(class AAllyMember* Pawn) override;
	
};
