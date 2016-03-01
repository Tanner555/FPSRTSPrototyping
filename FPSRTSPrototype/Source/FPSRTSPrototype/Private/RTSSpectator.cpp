// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSRTSPrototype.h"
#include "RTSSpectator.h"
#include "AllyMember.h"
#include "PartyManagerCPP.h"


//GETTERS
TEnumAsByte<EFactions> ARTSSpectator::getSpectatorFaction() {
	return SpectatorFaction;
}

TEnumAsByte<ECommanders> ARTSSpectator::getSpectatorGeneral() {
	return SpectatorGeneral;
}

class AAllyMember* ARTSSpectator::getSpectatorAlly() {
	return SpectatorAlly;
}

class APartyManagerCPP* ARTSSpectator::getSpectatorPartyManager() {
	APartyManagerCPP* retParty = nullptr;
	ARTSProtoGameMode* gamemode = Cast<ARTSProtoGameMode>(UGameplayStatics::GetGameMode(this));
	if (gamemode) {
		retParty = gamemode->getPartyManager(SpectatorAlly);
	}
	return retParty;
}

//SETTERS
void ARTSSpectator::setSpectatorFaction(TEnumAsByte<EFactions> faction) {
	SpectatorFaction.operator==(faction.GetValue());
}

void ARTSSpectator::setSpectatorGeneral(TEnumAsByte<ECommanders> general) {
	SpectatorGeneral.operator==(general.GetValue());
}

void ARTSSpectator::setSpectatorAlly(class AAllyMember* ally) {
	if (ally != nullptr) {
		SpectatorAlly = ally;
	}
}