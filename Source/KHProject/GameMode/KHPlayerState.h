// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIsDown;


public:
	bool IsDown() {return bIsDown;}
	
};
