// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/KHPlayerController_Player.h"

#include "GameFramework/PlayerState.h"
#include "GameMode/KHGameMode_Lobby.h"
#include "GameMode/KHGameState_Lobby.h"
#include "UI/KHHUD_Lobby.h"
#include "UI/Widget/KHWidget_Lobby.h"


void AKHPlayerController_Player::BeginPlay()
{
	Super::BeginPlay();

	if (AKHGameState_Lobby* pLobby = Cast<AKHGameState_Lobby>(GetWorld()->GetGameState()))
	{
		if (AKHHUD_Lobby* pHUD = Cast<AKHHUD_Lobby>(GetHUD()))
		{
			if (UKHWidget_Lobby* pLobbyWidget = Cast<UKHWidget_Lobby>(pHUD->GetHUDWidget()))
			{
				pLobbyWidget->ConnectGameState();
			}
		}
	}
}

void AKHPlayerController_Player::Server_PlayerReady_Implementation()
{
	if (AKHGameMode_Lobby* pGameMode = Cast<AKHGameMode_Lobby>(GetWorld()->GetAuthGameMode()))
	{
		FString PlayerGuid = PlayerState->GetPlayerName();
		pGameMode->NotifyPlayerReady(PlayerGuid);
	}


}
