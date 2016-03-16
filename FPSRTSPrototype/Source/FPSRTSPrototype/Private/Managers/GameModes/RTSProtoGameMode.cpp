// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "PartyManagerCPP.h"
#include "AllyMember.h"
#include "RTSSpectator.h"
#include "RTSProtoGameMode.h"

ARTSProtoGameMode::ARTSProtoGameMode() : Super() {
	//EXAMPLE PURPOSES ONLY
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Blueprint'/Game/RTSFPSBlueprints/Managers/PartyManagerCPP_BP.PartyManagerCPP_BP'"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;
	//DefaultPawnClass = APartyManagerCPP::StaticClass();
	//HUDClass = AFPSRTSPrototypeHUD::StaticClass();
}

void ARTSProtoGameMode::BeginPlay() {
	Super::BeginPlay();
	
	APartyManagerCPP* firstGeneralFound = FindGenerals(false, nullptr);
	if (firstGeneralFound == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No General could be found!"));
	}
	else {
		for (int i = 0; i < GeneralMembers.Num(); i++) {
			if (GeneralMembers[i]->generalCommander.operator==(ECommanders::Commander_01)) {
				SetGeneralInCommand(GeneralMembers[i]);
			}
		}
	}

	if (generalInCommand == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("There is no Commander_01 in the scene!"));
	}
	
	if (AllyFaction.Num() <= 0) {
		AllyFaction.Add(ECommanders::Commander_01);
	}
	if (EnemyFaction.Num() <= 0) {
		EnemyFaction.Add(ECommanders::Commander_02);
	}

	ResetGameModeStats();
	InitializeGameModeValues();
	//OnTestDelegate.AddDynamic(this, &AMyPlayerController::TestFunction);
	//OnTestDelegate.Broadcast();
}

void ARTSProtoGameMode::Tick(float DeltaTime) {
	if (GetMatchState().operator==(ERTSGameState::EWaitingToStart)) {
		waitingTillBeginMatch();
	}
	else if (GetMatchState().operator==(ERTSGameState::EPlaying)) {
		playTheMatch();
	}
	
	
}

//GameModeSetupFunctions
APartyManagerCPP* ARTSProtoGameMode::FindGenerals(bool pendingLeave, class APartyManagerCPP* generalLeaving) {
	GeneralActors.Empty();
	GeneralMembers.Empty();
	UGameplayStatics::GetAllActorsOfClass(this, APartyManagerCPP::StaticClass(), GeneralActors);
	for (int i = 0; i < GeneralActors.Num(); i++) {
		APartyManagerCPP* generalMember = Cast<APartyManagerCPP>(GeneralActors[i]);
		if (pendingLeave) {
			if (GeneralActors[i] != generalLeaving) {
				GeneralMembers.Add(generalMember);
			}
		}
		else {
			GeneralMembers.Add(generalMember);
		}
	}

	UpdateGeneralStatuses();

	if (GeneralMembers.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("No partyMembers in Scene"));
		return nullptr;
	}
	else {
		APartyManagerCPP* FirstGeneralFound = GeneralMembers[0];
		return FirstGeneralFound;
	}
}

void ARTSProtoGameMode::SetGeneralInCommand(APartyManagerCPP* setToCommand) {
	int32 generalIndex = GeneralMembers.Find(setToCommand);
	if (GeneralMembers.IsValidIndex(generalIndex)) {
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->UnPossess();
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(setToCommand);
		generalInCommand = setToCommand;
		generalInCommandActor = Cast<AActor>(setToCommand);
	}
	UpdateGeneralStatuses();
	UpdateSpecStats();
}

void ARTSProtoGameMode::UpdateGeneralStatuses() {
	if (GeneralMembers.Num() > 0) {
		for (int i = 0; i < GeneralMembers.Num(); i++) {
			int32 allyFactionIndex = AllyFaction.Find(GeneralMembers[i]->generalCommander);
			int32 enemyFactionIndex = EnemyFaction.Find(GeneralMembers[i]->generalCommander);
			if (AllyFaction.IsValidIndex(allyFactionIndex)) {
				GeneralMembers[i]->generalFaction.operator=(EFactions::Faction_Allies);
			}
			else if (EnemyFaction.IsValidIndex(enemyFactionIndex)) {
				GeneralMembers[i]->generalFaction.operator=(EFactions::Faction_Enemies);
			}
		}
	}
}

int32 ARTSProtoGameMode::GetAllyFactionPlayerCount(AAllyMember* teamMember) {
	int32 playerCount = 0;
	TEnumAsByte<EFactions> Faction = getAllyFaction(teamMember);
	TArray<AActor*> playerActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAllyMember::StaticClass(), playerActors);
	for (int i = 0; i < playerActors.Num(); i++) {
		AAllyMember* Ally = Cast<AAllyMember>(playerActors[i]);
		if (Ally->getAllyFaction().operator==(Faction.GetValue())) {
			playerCount++;
		}
	}
	return playerCount;
}

//Adding new versions of playercount and partymanager functions for flexibility

int32 ARTSProtoGameMode::GetFactionPlayerCount(TEnumAsByte<EFactions> Faction) {
	int32 playerCount = 0;
	TArray<AActor*> playerActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAllyMember::StaticClass(), playerActors);
	for (int i = 0; i < playerActors.Num(); i++) {
		AAllyMember* Ally = Cast<AAllyMember>(playerActors[i]);
		if (Ally->getAllyFaction().operator==(Faction.GetValue())) {
			playerCount++;
		}
	}
	return playerCount;
}

int32 ARTSProtoGameMode::GetGeneralPlayerCount(TEnumAsByte<ECommanders> General) {
	int32 playerCount = 0;
	TArray<AActor*> playerActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAllyMember::StaticClass(), playerActors);
	for (int i = 0; i < playerActors.Num(); i++) {
		AAllyMember* Ally = Cast<AAllyMember>(playerActors[i]);
		if (Ally->generalCommander.operator==(General.GetValue())) {
			playerCount++;
		}
	}
	return playerCount;
}

APartyManagerCPP* ARTSProtoGameMode::getPartyManagerFromECommander(TEnumAsByte<ECommanders> General) {
	for (int i = 0; i < GeneralMembers.Num(); i++) {
		if (GeneralMembers[i]->generalCommander.operator==(General.GetValue())) {
			return GeneralMembers[i];
		}
	}
	return nullptr;
}

//GetAllyGeneralPlayerCount

int32 ARTSProtoGameMode::GetAllyGeneralPlayerCount(AAllyMember* teamMember) {
	int32 playerCount = 0;
	TEnumAsByte<ECommanders> General = teamMember->generalCommander;
	TArray<AActor*> playerActors;
	UGameplayStatics::GetAllActorsOfClass(this, AAllyMember::StaticClass(), playerActors);
	for (int i = 0; i < playerActors.Num(); i++) {
		AAllyMember* Ally = Cast<AAllyMember>(playerActors[i]);
		if (Ally->generalCommander.operator==(General.GetValue())) {
			playerCount++;
		}
	}
	return playerCount;
}

APartyManagerCPP* ARTSProtoGameMode::getPartyManager(AAllyMember* teamMember) {
	for (int i = 0; i < GeneralMembers.Num(); i++) {
		if (GeneralMembers[i]->generalCommander.operator==(teamMember->generalCommander.GetValue())) {
			return GeneralMembers[i];
		}
	}
	return nullptr;
}

TArray<APartyManagerCPP*> ARTSProtoGameMode::getPartyManagers(TEnumAsByte<EFactions> Faction) {
	TArray<APartyManagerCPP*> partymanagers;
	for (int32 i = 0; i < GeneralMembers.Num(); i++) {
		if (GeneralMembers[i]->generalFaction.operator==(Faction.GetValue())) {
			partymanagers.Add(GeneralMembers[i]);
		}
	}
	return partymanagers;
}

TEnumAsByte<EFactions> ARTSProtoGameMode::getAllyFaction(AAllyMember* teamMember) {
	int32 allyFactionIndex = AllyFaction.Find(teamMember->generalCommander);
	int32 enemyFactionIndex = EnemyFaction.Find(teamMember->generalCommander);
	if (AllyFaction.IsValidIndex(allyFactionIndex)) {
		return EFactions::Faction_Allies;
	}
	else if (EnemyFaction.IsValidIndex(enemyFactionIndex)) {
		return EFactions::Faction_Enemies;
	}
	else {
		return EFactions::Faction_Default;
	}
}

void ARTSProtoGameMode::CallGameOverEvent(TEnumAsByte<ECommanders> callingCommander) {
	
}

//Specator Events
TSubclassOf<class ARTSSpectator> ARTSProtoGameMode::getSpectatorSubClass(int32 index) {
	TSubclassOf<ARTSSpectator> retSpec = nullptr;
	//ARTSSpectator* retSpec = nullptr;
	int32 SpecIndex = Spectators.Find(Spectators[index]);
	if (Spectators.IsValidIndex(SpecIndex)) {
		retSpec = Spectators[index];
	}
	return retSpec;
}

class ARTSSpectator* ARTSProtoGameMode::getSpectatorClass(int32 index) {
	ARTSSpectator* retSpecClass = nullptr;
	TSubclassOf<ARTSSpectator> specSub = getSpectatorSubClass(index);
	if (specSub != nullptr) {
		retSpecClass = Cast<ARTSSpectator>(specSub);
	}

	return retSpecClass;
}

void ARTSProtoGameMode::setSpecFaction(int32 specIndex, TEnumAsByte<EFactions> faction) {
	ARTSSpectator* spectator = getSpectatorClass(specIndex);
	if (spectator != nullptr) {
		spectator->setSpectatorFaction(faction);
	}
}

void ARTSProtoGameMode::setSpecGeneral(int32 specIndex, TEnumAsByte<ECommanders> general) {
	ARTSSpectator* spectator = getSpectatorClass(specIndex);
	if (spectator != nullptr) {
		spectator->setSpectatorGeneral(general);
	}
}

void ARTSProtoGameMode::setSpecAlly(int32 specIndex, class AAllyMember* ally) {
	ARTSSpectator* spectator = getSpectatorClass(specIndex);
	if (spectator != nullptr) {
		spectator->setSpectatorAlly(ally);
	}
}

void ARTSProtoGameMode::UpdateSpecStats() {
	for (int32 i = 0; i < Spectators.Num(); i++) {
		if (getSpectatorClass(i) != nullptr && generalInCommand != nullptr) {
			setSpecFaction(i,getAllyFaction(generalInCommand->allyInCommand));
			setSpecGeneral(i, generalInCommand->generalCommander);
			setSpecAlly(i, generalInCommand->allyInCommand);
		}
	}
}

//Kills and Points Getters
int32 ARTSProtoGameMode::GetFactionKills(bool CalculateAccurateResults, TEnumAsByte<EFactions> Faction) {
	int32& FactionKills = Faction.operator==(EFactions::Faction_Allies) ? Allies_Kills : Enemies_Kills;
	if (!Faction.operator==(EFactions::Faction_Allies) && !Faction.operator==(EFactions::Faction_Enemies)) {
		return 0;
	}

	if (CalculateAccurateResults) {
		for (int32 i = 0; i < GeneralMembers.Num(); i++) {
			if (GeneralMembers[i]->generalFaction.operator==(Faction.GetValue())) {
				FactionKills += GeneralMembers[i]->GetPartyKillCount();
			}
		}
	}

	return FactionKills;
}
int32 ARTSProtoGameMode::GetFactionPoints(bool CalculateAccurateResults, TEnumAsByte<EFactions> Faction) {
	int32& FactionPoints = Faction.operator==(EFactions::Faction_Allies) ? Allies_Points : Enemies_Points;
	if (!Faction.operator==(EFactions::Faction_Allies) && !Faction.operator==(EFactions::Faction_Enemies)) {
		return 0;
	}

	if (CalculateAccurateResults) {
		for (int32 i = 0; i < GeneralMembers.Num(); i++) {
			if (GeneralMembers[i]->generalFaction.operator==(Faction.GetValue())) {
				FactionPoints += GeneralMembers[i]->GetPartyPointsScored();
			}
		}
	}

	return FactionPoints;
}
int32 ARTSProtoGameMode::GetFactionDeaths(bool CalculateAccurateResults, TEnumAsByte<EFactions> Faction) {
	int32& FactionDeaths = Faction.operator==(EFactions::Faction_Allies) ? Allies_Deaths : Enemies_Deaths;
	if (!Faction.operator==(EFactions::Faction_Allies) && !Faction.operator==(EFactions::Faction_Enemies)) {
		return 0;
	}

	if (CalculateAccurateResults) {
		for (int32 i = 0; i < GeneralMembers.Num(); i++) {
			if (GeneralMembers[i]->generalFaction.operator==(Faction.GetValue())) {
				FactionDeaths += GeneralMembers[i]->GetPartyDeathCount();
			}
		}
	}

	return FactionDeaths;
}

//Reset or Update Stats
void ARTSProtoGameMode::UpdateGameModeStats() {
	TEnumAsByte<EFactions> allyFac;
	TEnumAsByte<EFactions> enemyFac;
	allyFac.operator=(EFactions::Faction_Allies);
	enemyFac.operator=(EFactions::Faction_Enemies);
	GetFactionKills(true, allyFac);
	GetFactionKills(true, enemyFac);
	GetFactionPoints(true, allyFac);
	GetFactionPoints(true, enemyFac);
	GetFactionDeaths(true, allyFac);
	GetFactionDeaths(true, enemyFac);
}

void ARTSProtoGameMode::ResetGameModeStats() {
	Allies_Kills = 0;
	Enemies_Kills = 0;
	Allies_Points = 0;
	Enemies_Points = 0;
	Allies_Deaths = 0;
	Enemies_Deaths = 0;
}

void ARTSProtoGameMode::InitializeGameModeValues() {
	DefaultKillPoints = 3;
	DefaultFriendlyFirePoints = -1;
	DefaultMatchTimeLimit = 60.0f * 5.0f;
	DefaultMatchStartingTime = GetWorld()->GetTimeSeconds() + DefaultMatchTimeLimit;
	TEnumAsByte<ERTSGameState> waitingstate;
	waitingstate.operator=(ERTSGameState::EWaitingToStart);
	SetMatchState(waitingstate);
	
}

int32 ARTSProtoGameMode::GetPendingReward(class AAllyMember* receiver, TEnumAsByte<ERTSRewardTypes> rewardType) {
	return DefaultKillPoints;

}

int32 ARTSProtoGameMode::GetPendingPunishment(class AAllyMember* receiver, TEnumAsByte<ERTSPunishmentTypes> punishType) {
	return DefaultFriendlyFirePoints;
	
}

//GameState Getter and Setter

TEnumAsByte<ERTSGameState> ARTSProtoGameMode::GetMatchState() {
	return MatchState;

}

void ARTSProtoGameMode::SetMatchState(TEnumAsByte<ERTSGameState> setmatchstate) {
	MatchState = setmatchstate;

}

//virtual play state methods handling the match state inside of tick function

void ARTSProtoGameMode::playTheMatch() {
	//if (MatchRemainingTime > 0) {
	//	MatchRemainingTime--;
	//}
	MatchRemainingTime = DefaultMatchStartingTime - GetWorld()->GetTimeSeconds();
	RemainingMinutes = (int32)(MatchRemainingTime / 60.f);
	RemainingSeconds = ((int32)MatchRemainingTime) % 60;
	//if (GetWorld()->GetTimeSeconds() - DefaultMatchStartingTime > DefaultMatchTimeLimit)
	if (MatchRemainingTime <= 0) {
		TEnumAsByte<ERTSGameState> gameover;
		gameover.operator=(ERTSGameState::EGameOver);
		SetMatchState(gameover);
	}
	
	//if (MatchRemainingTime <= 0) {
	//	TEnumAsByte<ERTSGameState> gameover;
	//	gameover.operator=(ERTSGameState::EGameOver);
	//	SetMatchState(gameover);
	//}
}

void ARTSProtoGameMode::waitingTillBeginMatch() {
	if (GetMatchState().operator==(ERTSGameState::EWaitingToStart)) {
		TEnumAsByte<ERTSGameState> playgame;
		playgame.operator=(ERTSGameState::EPlaying);
		SetMatchState(playgame);
	}
	
}
