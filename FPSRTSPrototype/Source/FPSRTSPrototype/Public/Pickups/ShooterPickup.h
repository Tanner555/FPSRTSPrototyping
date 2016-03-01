// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ShooterPickup.generated.h"

class AllyMember;

UCLASS()
class FPSRTSPROTOTYPE_API AShooterPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterPickup(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	
	// Called every frame
	//virtual void Tick( float DeltaSeconds ) override;

	/** pickup on touch */
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

	/** check if pawn can use this pickup */
	virtual bool CanBePickedUp(class AAllyMember* TestPawn) const;

	/** initial setup */
	virtual void BeginPlay() override;

private:
	/** FX component */
	UPROPERTY(VisibleDefaultsOnly, Category = Effects)
		UParticleSystemComponent* PickupPSC;
protected:

	/** FX of active pickup */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* ActiveFX;

	/** FX of pickup on respawn timer */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem* RespawningFX;

	/** sound played when player picks it up */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		USoundCue* PickupSound;

	/** sound played on respawn */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		USoundCue* RespawnSound;

	/** how long it takes to respawn? */
	UPROPERTY(EditDefaultsOnly, Category = Pickup)
		float RespawnTime;

	/** is it ready for interactions? */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsActive)
		uint32 bIsActive : 1;

	/* The character who has picked up this pickup */
	UPROPERTY(Transient, Replicated)
	class AAllyMember* PickedUpBy;

	/** Handle for efficient management of RespawnPickup timer */
	FTimerHandle TimerHandle_RespawnPickup;

	UFUNCTION()
		void OnRep_IsActive();

	/** give pickup */
	virtual void GivePickupTo(class AAllyMember* Pawn);

	/** handle touches */
	void PickupOnTouch(class AAllyMember* Pawn);

	/** show and enable pickup */
	virtual void RespawnPickup();

	/** show effects when pickup disappears */
	virtual void OnPickedUp();

	/** show effects when pickup appears */
	virtual void OnRespawned();

	/** blueprint event: pickup disappears */
	UFUNCTION(BlueprintImplementableEvent)
		void OnPickedUpEvent();

	/** blueprint event: pickup appears */
	UFUNCTION(BlueprintImplementableEvent)
		void OnRespawnEvent();

protected:
	/** Returns PickupPSC subobject **/
	FORCEINLINE UParticleSystemComponent* GetPickupPSC() const { return PickupPSC; }
	
};
