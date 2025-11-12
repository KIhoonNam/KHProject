// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KHPlayerController_Player.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHPlayerController_Player : public APlayerController
{
	GENERATED_BODY()
	


public:
	virtual void BeginPlay() override;
	


public:
	UFUNCTION(Server,Reliable)
	void Server_PlayerReady();
};
