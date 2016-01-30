// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ShootyShot.h"
#include "ShootyShotGameMode.h"
#include "ShootyShotHUD.h"
#include "ShootyShotCharacter.h"

AShootyShotGameMode::AShootyShotGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AShootyShotHUD::StaticClass();
}
