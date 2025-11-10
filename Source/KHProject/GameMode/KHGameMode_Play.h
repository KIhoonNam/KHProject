// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KHGameMode_Play.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHGameMode_Play : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	void CheckForGameOver();
	
};
