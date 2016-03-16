// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSRTSPrototypeCharacter.h"
#include "RTSProtoGameMode.h"
#include "AllyMember.generated.h"

class FPSRTSPrototypeCharacter;
class PartyManagerCPP;

/**
 * 
 */
UCLASS()
class FPSRTSPROTOTYPE_API AAllyMember : public AFPSRTSPrototypeCharacter
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:

	AAllyMember();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable,Category = "InputCommands")
	void AllyFire();
	UFUNCTION(BlueprintCallable, Category = "InputCommands")
	void AllyMoveForward(float Val);
	UFUNCTION(BlueprintCallable, Category = "InputCommands")
	void AllyMoveRight(float Val);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllySetup")
	TEnumAsByte<ECommanders> generalCommander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllySetup")
	USkeletalMeshComponent* thirdpersonmesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AllySetup")
	ARTSProtoGameMode* gamemode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllySetup")
	class UBehaviorTree* AllyBehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllySetup")
	UParticleSystem* deathSparks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllySetup")
	USoundBase* deathSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AllyBattleStatus")
	bool isTargetingEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AllyBattleStatus")
	AAllyMember* targetedEnemy;
private:
	float AllyHealth;
	float AllyMaxHealth;
	int32 CurrentAmmo;
	float baseEnemyDamage;
	bool AllyCanFire;
	bool ExecutingShootingBehavior;
	bool wantsFreedomToMove;
	float freeMoveThreshold;
	int32 MaxAmmo;
	FTimerHandle defaultShootTimer;
	float DefaultShootDelay;
	//Keep track of kills and points for gamemode
	int32 Kills;
	int32 Points;
	int32 Deaths;

public:
	UPROPERTY(EditDefaultsOnly, Category = "AttackAttribute")
	TSubclassOf<UDamageType> AllyDamageType;
	
	//AllyAttributes
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	float GetDamageRate(AActor* attacker);
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	bool isAlive() const;
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	bool CanFire() const;
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	float getAllyHealth() const;
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	float getAllyMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	int32 getCurrentAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	int32 GetMaxAmmo() const;
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	bool isExeShootBehavior() const;
	//Easier Add to Attribute replacement functions
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	void addAllyHealth(float addToHealth);
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	void addAllyCurrentAmmo(int32 addToCurrentAmmo);
	//End
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
		bool isEnemyFor(AAllyMember* player);
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
		bool isTargettingEnemy();
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
		bool wantsFreeMovement() const;
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
		void toggleFreeMovement();
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
		TEnumAsByte<EFactions> getAllyFaction();
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	class APartyManagerCPP* getPartyManager();
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	int32 GetFactionPlayerCount();
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	int32 GetGeneralPlayerCount();
	//Kills and Points
	//Getters
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	int32 GetKillCount();
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	int32 GetPointsScored();
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	int32 GetDeathCount();
	//Setters
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	void AddToKillCount(int32 killNum);
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	void AddToPointsScored(int32 scoreNum);
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	void AddToDeathCount(int32 deathNum);
	//Reset Deaths,Points, and Kills
	UFUNCTION(BlueprintCallable, Category = "AllyAttribute")
	void ResetAllyStats();
	//AllyActions
	UFUNCTION(BlueprintCallable, Category = "AllyActions")
	FHitResult PerformRaycastHit(FVector WorldLocation, FVector WorldDirection, bool& isSuccessful);
	UFUNCTION(BlueprintCallable, Category = "AllyActions")
		void StartShootingBehavior();
	UFUNCTION(BlueprintCallable, Category = "AllyActions")
		void StopShootingBehavior();
};
