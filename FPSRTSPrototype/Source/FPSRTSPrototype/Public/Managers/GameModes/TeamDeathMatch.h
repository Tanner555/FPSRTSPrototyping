// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Managers/GameModes/RTSProtoGameMode.h"
#include "TeamDeathMatch.generated.h"

/**
 * 
 */
UCLASS()
class FPSRTSPROTOTYPE_API ATeamDeathMatch : public ARTSProtoGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, Category = "Spawning")
	//TSubclassOf<class APickup> WhatToSpawn;
	//GameModeSpawning Function
	//UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Timers)
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDM Spawning Setup")
	FVector spawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDM Spawning Setup")
	int32 PlayersPerTeam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDM Spawning Setup")
	int32 ReinforcementThresholdNum;
	

private:
	FTimerHandle allyTimeHandle;
	FTimerHandle enemyTimeHandle;

	UFUNCTION(BlueprintCallable, Category = SpawningFactionMembers)
	void SpawnAlliesMembers();
	UFUNCTION(BlueprintCallable, Category = SpawningFactionMembers)
	void SpawnEnemiesMembers();
	UFUNCTION(BlueprintCallable, Category = SpawningFactionMembers)
	bool ShouldSpawnFactionMember(TEnumAsByte<EFactions> Faction);
	bool isTimeToSpawnReinforcements(TEnumAsByte<EFactions> Faction);
	UFUNCTION(BlueprintCallable, Category = SpawningFactionMembers)
	void SpawnFactionMember(TEnumAsByte<EFactions> Faction, FVector SpawnerLocation);
};
