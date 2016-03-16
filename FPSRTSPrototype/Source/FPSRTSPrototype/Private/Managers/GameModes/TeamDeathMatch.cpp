// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "TeamDeathMatch.h"
#include "AllyMember.h"
#include "PartyManagerCPP.h"

void ATeamDeathMatch::BeginPlay() {
	Super::BeginPlay();
	//set timer by event 3 seconds looping true
	GetWorldTimerManager().SetTimer(allyTimeHandle, this, &ATeamDeathMatch::SpawnAlliesMembers, 3, true);
	GetWorldTimerManager().SetTimer(enemyTimeHandle, this, &ATeamDeathMatch::SpawnEnemiesMembers, 3, true);
	if (PlayersPerTeam == 0 || ReinforcementThresholdNum == 0) {
		PlayersPerTeam = 10;
		ReinforcementThresholdNum = 2;
	}
}

//GameModeSpawning Function
void ATeamDeathMatch::SpawnAlliesMembers()
{
	TEnumAsByte<EFactions> fAlly; 
	fAlly.operator=(EFactions::Faction_Allies);
	if (ShouldSpawnFactionMember(fAlly) && !isTimeToSpawnReinforcements(fAlly)) {
		SpawnFactionMember(fAlly, spawnLocation);
	}
}

void ATeamDeathMatch::SpawnEnemiesMembers()
{
	TEnumAsByte<EFactions> fEnemy; 
	fEnemy.operator=(EFactions::Faction_Enemies);
	if (ShouldSpawnFactionMember(fEnemy) && !isTimeToSpawnReinforcements(fEnemy)) {
		SpawnFactionMember(fEnemy, spawnLocation);
	}
}

bool ATeamDeathMatch::isTimeToSpawnReinforcements(TEnumAsByte<EFactions> Faction) {
	bool isPlaying = GetMatchState().operator==(ERTSGameState::EPlaying);
	bool lessThanThreshold = GetFactionPlayerCount(Faction) < ReinforcementThresholdNum;
	if (isPlaying && lessThanThreshold) {
		while (GetFactionPlayerCount(Faction) < PlayersPerTeam) {
			SpawnFactionMember(Faction, spawnLocation);
		}
	}
	return isPlaying && lessThanThreshold;
}

bool ATeamDeathMatch::ShouldSpawnFactionMember(TEnumAsByte<EFactions> Faction) {
	bool isPlaying = GetMatchState().operator==(ERTSGameState::EPlaying);
	bool lessThanTeamNum = GetFactionPlayerCount(Faction) < PlayersPerTeam;
	return isPlaying && lessThanTeamNum;
}

void ATeamDeathMatch::SpawnFactionMember(TEnumAsByte<EFactions> Faction, FVector SpawnerLocation) {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	FVector SpawnLocation = UNavigationSystem::GetRandomReachablePointInRadius(this, SpawnerLocation, 1000, (ANavigationData*)0, 0);
	FRotator SpawnRotation;
	SpawnRotation.Yaw = 0;
	SpawnRotation.Pitch = 0;
	SpawnRotation.Roll = 0;
	if (AlliesSpawnClass && EnemiesSpawnClass) {
		if (Faction.operator==(EFactions::Faction_Allies)) {
			AAllyMember* const SpawnedAlly = GetWorld()->SpawnActor<AAllyMember>(AlliesSpawnClass, SpawnLocation, SpawnRotation, SpawnParams);
			SpawnedAlly->SpawnDefaultController();
			AAllyMember* firstAlly = generalInCommand->FindPartyMembers(false, nullptr);
			generalInCommand->SetAllyInCommand(firstAlly);
		}
		else if (Faction.operator==(EFactions::Faction_Enemies)) {
			AAllyMember* const SpawnedEnemy = GetWorld()->SpawnActor<AAllyMember>(EnemiesSpawnClass, SpawnLocation, SpawnRotation, SpawnParams);
			SpawnedEnemy->SpawnDefaultController();
		}
	}
}