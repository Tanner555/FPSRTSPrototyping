// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FPSRTSPrototype.h"
#include "FPSRTSPrototypeGameMode.h"
#include "FPSRTSPrototypeHUD.h"
#include "FPSRTSPrototypeCharacter.h"

AFPSRTSPrototypeGameMode::AFPSRTSPrototypeGameMode():Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSRTSPrototypeHUD::StaticClass();
}
