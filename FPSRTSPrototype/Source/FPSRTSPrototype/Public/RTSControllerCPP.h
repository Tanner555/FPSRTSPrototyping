// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "RTSControllerCPP.generated.h"

class AllyMember;

/**
 * 
 */
UCLASS()
class FPSRTSPROTOTYPE_API ARTSControllerCPP : public AAIController
{
	GENERATED_BODY()

	ARTSControllerCPP(const class FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;
	
public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "ControllerSetup")
	UBehaviorTreeComponent* BehaviorTreeComp;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "ControllerSetup")
	UBlackboardComponent* BlackBoardComp;
	//KeyNames
	//Vectors
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName moveCommandLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName LocationOfSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName WanderPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName WanderLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName targetedEnemyLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName Destination;
	//Bools
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName isTargetingEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName moveCommandActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName HasHeardSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName NeedAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName HasLosToEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
	FName WantsToMove;
	//Object Actors
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName targetedEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName SelfActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName PlayerActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName AllyActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
		FName PatrolPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControllerSetup")
	FName Enemy;

	virtual void BeginInactiveState() override;
	void Respawn();
	//Set Auto Moving AI to false if true
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void setWantsToMove(bool shouldMove);
	/* Finds the closest enemy and sets them as current target */
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void FindClosestEnemy();

	UFUNCTION(BlueprintCallable, Category = Behavior)
	bool FindClosestEnemyWithLOS(class AAllyMember* ExcludeEnemy);

	//SHOOTERAICONTROLLER
	
	public:

	// Begin AController interface
	virtual void GameHasEnded(class AActor* EndGameFocus = NULL, bool bIsWinner = false) override;
	// End APlayerController interface

	void CheckAmmo(class AAllyMember* Ally);

	void SetEnemy(class AAllyMember* AllyPawn);

	class AAllyMember* GetEnemy() const;

	/* If there is line of sight to current enemy, start firing at them */
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void ShootEnemy();

	bool HasWeaponLOSToEnemy(AActor* InEnemyActor, const bool bAnyEnemy) const;

	// Begin AAIController interface
	/** Update direction AI is looking based on FocalPoint */
	//virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;
	// End AAIController interface

protected:
	// Check of we have LOS to a character
	//bool LOSTrace(AShooterCharacter* InEnemyChar) const;

	int32 EnemyKeyID;
	int32 NeedAmmoKeyID;

	/** Handle for efficient management of Respawn timer */
	FTimerHandle TimerHandle_Respawn;

public:
	/** Returns BlackboardComp subobject **/
	//FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	/** Returns BehaviorComp subobject **/
	//FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }


};
