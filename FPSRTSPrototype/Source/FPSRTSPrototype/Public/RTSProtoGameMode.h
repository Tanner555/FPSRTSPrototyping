// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "RTSProtoGameMode.generated.h"

class AllyMember;
class PartyManagerCPP;
class ShooterPickup;
class RTSSpectator;

//In order for enums to be declared in another class, you must get rid
//of the uint8 and class words and declare it outside of the class.
//Declare an instance of the enum using TEnumAsByte.

UENUM(BlueprintType)
enum ECommanders {
	Commander_01,
	Commander_02,
	Commander_03,
	Commander_04,
	Commander_05,
	Commander_06,
};

UENUM(BlueprintType)
enum EFactions {
	Faction_Allies,
	Faction_Enemies,
	Faction_Default,
};

UENUM(BlueprintType)
enum ERTSGameState {
	EWaitingToStart,
	EPlaying,
	EGameIsPaused,
	EGameOver,
	EWon,
	EUnknown
};

UENUM(BlueprintType)
enum ERTSRewardTypes {
	Reward_Kill,
	
};

UENUM(BlueprintType)
enum ERTSPunishmentTypes {
	Punishment_KilledAnAlly,

};
		
/**
 * 
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTestDelegate, PARAMTYPE, PARAMNAME);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(TestingDelegate);

UCLASS()
class FPSRTSPROTOTYPE_API ARTSProtoGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	//EditAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameModeSetup)
		TArray<TEnumAsByte<ECommanders>> AllyFaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameModeSetup)
		TArray<TEnumAsByte<ECommanders>> EnemyFaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameModeSetup)
		TArray<TSubclassOf<class ARTSSpectator>> Spectators;
	//ReadOnly
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameModeSetup)
		TArray<AActor*> GeneralActors;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameModeSetup)
		TArray<class APartyManagerCPP*> GeneralMembers;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameModeSetup)
		class APartyManagerCPP* generalInCommand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameModeSetup)
		AActor* generalInCommandActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PartySetup)
		int32 TargetKillCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PartySetup)
		int32 CurrentEnemyCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PartySetup)
		int32 TargetGoal = 2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PartySetup)
		int32 RoundScaleMultiplier = 2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PartySetup)
		int32 AmmoInLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PartySetup)
		bool LostGame = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PartySetup)
		AActor* enemySpawner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PartySetup)
	TArray<class AShooterPickup*> LevelPickups;
	//GameTimer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timer)
	int32 RemainingMinutes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Timer)
	int32 RemainingSeconds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timer)
	float DefaultMatchTimeLimit;
	//GameModeFunctions
	ARTSProtoGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,Category = GameModeSetup)
	class APartyManagerCPP* FindGenerals(bool pendingLeave, class APartyManagerCPP* generalLeaving);
	UFUNCTION(BlueprintCallable, Category = GameModeSetup)
	void SetGeneralInCommand(class APartyManagerCPP* setToCommand);
	UFUNCTION(BlueprintCallable, Category = GameModeSetup)
		void UpdateGeneralStatuses();
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	void CallGameOverEvent(TEnumAsByte<ECommanders> callingCommander);
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	int32 GetAllyFactionPlayerCount(class AAllyMember* teamMember);
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	int32 GetAllyGeneralPlayerCount(class AAllyMember* teamMember);
	//Adding new versions of playercount and partymanager functions for flexibility
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	int32 GetFactionPlayerCount(TEnumAsByte<EFactions> Faction);
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	int32 GetGeneralPlayerCount(TEnumAsByte<ECommanders> General);
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	class APartyManagerCPP* getPartyManagerFromECommander(TEnumAsByte<ECommanders> General);
	//End
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	class APartyManagerCPP* getPartyManager(class AAllyMember* teamMember);
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	TArray<class APartyManagerCPP*> getPartyManagers(TEnumAsByte<EFactions> Faction);
	UFUNCTION(BlueprintCallable, Category = GameModeEvent)
	TEnumAsByte<EFactions> getAllyFaction(class AAllyMember* teamMember);
	//Specator
	//Getters
	UFUNCTION(BlueprintCallable, Category = SpecatorEvents)
	TSubclassOf<class ARTSSpectator> getSpectatorSubClass(int32 index);
	UFUNCTION(BlueprintCallable, Category = SpecatorEvents)
	class ARTSSpectator* getSpectatorClass(int32 index);
	//Setters
	UFUNCTION(BlueprintCallable, Category = SpecatorEvents)
	void setSpecFaction(int32 specIndex, TEnumAsByte<EFactions> faction);
	UFUNCTION(BlueprintCallable, Category = SpecatorEvents)
	void setSpecGeneral(int32 specIndex, TEnumAsByte<ECommanders> general);
	UFUNCTION(BlueprintCallable, Category = SpecatorEvents)
	void setSpecAlly(int32 specIndex, class AAllyMember* ally);
	UFUNCTION(BlueprintCallable, Category = SpecatorEvents)
	void UpdateSpecStats();
	//Kills and Points
	//Getters
	UFUNCTION(BlueprintCallable, Category = GameModeStats)
	int32 GetFactionKills(bool CalculateAccurateResults, TEnumAsByte<EFactions> Faction);
	UFUNCTION(BlueprintCallable, Category = GameModeStats)
	int32 GetFactionPoints(bool CalculateAccurateResults, TEnumAsByte<EFactions> Faction);
	UFUNCTION(BlueprintCallable, Category = GameModeStats)
	int32 GetFactionDeaths(bool CalculateAccurateResults, TEnumAsByte<EFactions> Faction);
	//Reset or Update Stats
	UFUNCTION(BlueprintCallable, Category = GameModeStats)
	void UpdateGameModeStats();
	UFUNCTION(BlueprintCallable, Category = GameModeStats)
	void ResetGameModeStats();
	UFUNCTION(BlueprintCallable, Category = GameModeStats)
	void InitializeGameModeValues();
	//AllyPointSystem
	UFUNCTION(BlueprintCallable, Category = GameModeStats)
	int32 GetPendingReward(class AAllyMember* receiver, TEnumAsByte<ERTSRewardTypes> rewardType);
	UFUNCTION(BlueprintCallable, Category = GameModeStats)
	int32 GetPendingPunishment(class AAllyMember* receiver, TEnumAsByte<ERTSPunishmentTypes> punishType);
	//GameState Getter and Setter
	UFUNCTION(BlueprintCallable, Category = GameModeMatchStates)
	TEnumAsByte<ERTSGameState> GetMatchState();
	UFUNCTION(BlueprintCallable, Category = GameModeMatchStates)
	void SetMatchState(TEnumAsByte<ERTSGameState> setmatchstate);
protected:
	//PlayStates in tick function
	//UFUNCTION(BlueprintCallable, Category = GameModePlayStates)
	virtual void playTheMatch();
	//UFUNCTION(BlueprintCallable, Category = GameModePlayStates)
	virtual void waitingTillBeginMatch();
private:
	int32 Allies_Kills;
	int32 Enemies_Kills;
	int32 Allies_Points;
	int32 Enemies_Points;
	int32 Allies_Deaths;
	int32 Enemies_Deaths;
	//timers
	float MatchRemainingTime;
	float DefaultMatchStartingTime;

	//Point Rewards
	int32 DefaultKillPoints;
	//Point Consequences
	int32 DefaultFriendlyFirePoints;
	//Important GameState Instance
	TEnumAsByte<ERTSGameState> MatchState;

	//Testing Only
	//UPROPERTY(BlueprintAssignable, Category = "Test")
	//TestingDelegate OnTestDelegate;
};
