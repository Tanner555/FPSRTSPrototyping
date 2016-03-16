// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "RTSProtoGameMode.h"
#include "PartyManagerCPP.generated.h"

class FPSRTSPrototypeCharacter;
class AllyMember;

UCLASS()
class FPSRTSPROTOTYPE_API APartyManagerCPP : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APartyManagerCPP();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PartySetup)
	TEnumAsByte<ECommanders> generalCommander;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PartySetup)
	TEnumAsByte<EFactions> generalFaction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PartySetup)
	ARTSProtoGameMode* gamemode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PartySetup)
	FVector FollowAllyCameraOffset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PartySetup)
	AAllyMember* allyInCommand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PartySetup)
	AActor* allyInCommandActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PartySetup)
	TArray<AAllyMember*> PartyMembers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PartySetup)
	TArray<AActor*> PartyActorArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PartyAttributes)
		bool isInOverview;
	//PartyManager Functions
	//UFUNCTION(BlueprintNativeEvent, Category = BlueprintEvent)
		//void GetGeneralCommander();
	UFUNCTION(BlueprintCallable,Category = PartySetup)
	AAllyMember* FindPartyMembers(bool pendingAllyLeave, AAllyMember* allyToLeave);
	UFUNCTION(BlueprintCallable, Category = PartySetup)
	void SetAllyInCommand(AAllyMember* setToCommand);
	//Input Controls
	UFUNCTION(BlueprintCallable, Category = InputCommands)
		void AllyCommandJump();
	UFUNCTION(BlueprintCallable, Category = InputCommands)
		void AllyCommandStopJumping();
	UFUNCTION(BlueprintCallable, Category = InputCommands)
	void AllyCommandFire();
	UFUNCTION(BlueprintCallable, Category = InputCommands)
	void CheckSelectionRaycast();
	UFUNCTION(BlueprintCallable, Category = InputCommands)
	void AllyCommandMoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category = InputCommands)
	void AllyCommandMoveRight(float Value);
	UFUNCTION(BlueprintCallable, Category = InputCommands)
		void AllyCommandToggleFreeMovement();
	UFUNCTION(BlueprintCallable, Category = InputCommands)
		void PossessAllyAdd();
	UFUNCTION(BlueprintCallable, Category = InputCommands)
		void PossessAllySubtract();
	//Kills and Points
	//Getters
	UFUNCTION(BlueprintCallable, Category = PartyStats)
	int32 GetPartyKillCount();
	UFUNCTION(BlueprintCallable, Category = PartyStats)
	int32 GetPartyPointsScored();
	UFUNCTION(BlueprintCallable, Category = PartyStats)
	int32 GetPartyDeathCount();

private:
	int32 PartyKills;
	int32 PartyPoints;
	int32 PartyDeaths;
	//AIControllerClass* tempAIControllerRef;
};
