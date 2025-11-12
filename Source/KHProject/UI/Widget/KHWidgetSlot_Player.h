// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/KHGameState_Lobby.h"
#include "KHWidgetSlot_Player.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidgetSlot_Player : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	bool bIsReady;
	
public:
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerName;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerReady;
	

public:
	void SetSlots(FLobbyPlayerInfo InPlayerInfo);

	void SetSlotReady();
};
