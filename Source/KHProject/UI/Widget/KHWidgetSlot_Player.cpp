// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidgetSlot_Player.h"
#include "Character/KHCharacter_Player.h"
#include "TextBlock.h"
#include "DataTable/KHDataTable_WeaponData.h"




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

	if (Text_PlayerWeapon)
	{
		FString strWeapon = InPlayerInfo.m_eWeaponType == EWeaponType::None ? "" : EnumToString(InPlayerInfo.m_eWeaponType);
		Text_PlayerWeapon->SetText(FText::FromString(strWeapon));
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
