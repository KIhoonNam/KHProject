// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidgetSlot_Player.h"

#include "TextBlock.h"




void UKHWidgetSlot_Player::SetSlots(FLobbyPlayerInfo InPlayerInfo)
{
	PlayerName = InPlayerInfo.PlayerName;

	bIsReady = InPlayerInfo.bIsReady;

	if (Text_PlayerName)
	{
		Text_PlayerName->SetText(FText::FromString(PlayerName));
	}

	if (Text_PlayerReady)
	{
		FString strReady = bIsReady ? "Ready" : "Not Ready";
		Text_PlayerReady->SetText(FText::FromString(strReady));
	}
}

void UKHWidgetSlot_Player::SetSlotReady()
{
	bIsReady = bIsReady ? false : true;
	
	if (Text_PlayerReady)
	{
		Text_PlayerReady->SetText(FText::FromString("Ready"));
	}
}
