// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KHGameMode_Lobby.h"

#include "KHGameState_Lobby.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"


AKHGameMode_Lobby::AKHGameMode_Lobby()
{
}

void AKHGameMode_Lobby::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	FString PlayerName = NewPlayer->PlayerState->GetPlayerName();
	AKHGameState_Lobby* LobbyGameState = GetGameState<AKHGameState_Lobby>();
	if (LobbyGameState)
	{
		FLobbyPlayerInfo LobbyPlayerInfo;
		LobbyPlayerInfo.PlayerName = PlayerName;
		LobbyPlayerInfo.bIsReady = false;
		if (!LobbyGameState->arrPlayerNames.Contains(LobbyPlayerInfo))
		{
			LobbyGameState->arrPlayerNames.Emplace(LobbyPlayerInfo);
		}
		LobbyGameState->CurrentPlayers = LobbyGameState->PlayerArray.Num();
		LobbyGameState->OnRep_CurrentPlayers();
	}

	UpdateLobbyStatus();
}

void AKHGameMode_Lobby::Logout(AController* Exiting)
{
	FString PlayerName = Exiting->PlayerState->GetPlayerName();
	AKHGameState_Lobby* LobbyGameState = GetGameState<AKHGameState_Lobby>();
	if (LobbyGameState)
	{
		FLobbyPlayerInfo LobbyPlayerInfo;
		LobbyPlayerInfo.PlayerName = PlayerName;

		LobbyGameState->arrPlayerNames.Remove(LobbyPlayerInfo);
		
		LobbyGameState->CurrentPlayers = LobbyGameState->PlayerArray.Num();
		
		LobbyGameState->OnRep_CurrentPlayers();
	}
	
	UpdateLobbyStatus();
	
	Super::Logout(Exiting);
}

void AKHGameMode_Lobby::StartGame()
{
	GetWorld()->ServerTravel(MainGameMapPath, false, true);
}

void AKHGameMode_Lobby::UpdateLobbyStatus()
{
	
}

