// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdvancedHorrorAIGameMode.h"
#include "AdvancedHorrorAICharacter.h"
#include "UObject/ConstructorHelpers.h"

AAdvancedHorrorAIGameMode::AAdvancedHorrorAIGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
