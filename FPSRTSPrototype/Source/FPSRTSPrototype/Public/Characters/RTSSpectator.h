// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpectatorPawn.h"
#include "RTSProtoGameMode.h"
#include "RTSSpectator.generated.h"

class AllyMember;
class PartyManagerCPP;

/**
 * 
 */
UCLASS()
class FPSRTSPROTOTYPE_API ARTSSpectator : public ASpectatorPawn
{
	GENERATED_BODY()
	
private:

	//Spectator properties
	TEnumAsByte<EFactions> SpectatorFaction;
	TEnumAsByte<ECommanders> SpectatorGeneral;
	class AAllyMember* SpectatorAlly;

public:
	//Spectator Functions
	//Getters
	UFUNCTION(BlueprintCallable, Category = SpectatorSetup)
	TEnumAsByte<EFactions> getSpectatorFaction();
	UFUNCTION(BlueprintCallable, Category = SpectatorSetup)
	TEnumAsByte<ECommanders> getSpectatorGeneral();
	UFUNCTION(BlueprintCallable, Category = SpectatorSetup)
	class AAllyMember* getSpectatorAlly();
	UFUNCTION(BlueprintCallable, Category = SpectatorSetup)
	class APartyManagerCPP* getSpectatorPartyManager();
	//Setters
	UFUNCTION(BlueprintCallable, Category = SpectatorSetup)
	void setSpectatorFaction(TEnumAsByte<EFactions> faction);
	UFUNCTION(BlueprintCallable, Category = SpectatorSetup)
	void setSpectatorGeneral(TEnumAsByte<ECommanders> general);
	UFUNCTION(BlueprintCallable, Category = SpectatorSetup)
	void setSpectatorAlly(class AAllyMember* ally);
};
