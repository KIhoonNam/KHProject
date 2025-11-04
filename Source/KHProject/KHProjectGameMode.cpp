// Copyright Epic Games, Inc. All Rights Reserved.

#include "KHProjectGameMode.h"
#include "KHProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKHProjectGameMode::AKHProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
