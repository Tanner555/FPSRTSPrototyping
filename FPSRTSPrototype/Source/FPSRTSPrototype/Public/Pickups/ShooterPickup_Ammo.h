// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickups/ShooterPickup.h"
#include "ShooterPickup_Ammo.generated.h"

class AllyMember;

/**
 * 
 */
UCLASS()
class FPSRTSPROTOTYPE_API AShooterPickup_Ammo : public AShooterPickup
{
	GENERATED_BODY()

	AShooterPickup_Ammo(const FObjectInitializer& ObjectInitializer);
		/** check if pawn can use this pickup */
	virtual bool CanBePickedUp(class AAllyMember* TestPawn) const override;

	//bool IsForWeapon(UClass* WeaponClass);

protected:

	/** how much ammo does it give? */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		int32 AmmoClips;
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		int32 DefClipSize;
	/** which weapon gets ammo? */
	//UPROPERTY(EditDefaultsOnly, Category = Pickup)
		//TSubclassOf<AShooterWeapon> WeaponType;

	/** give pickup */
	virtual void GivePickupTo(AAllyMember* Pawn) override;
	
	
};
