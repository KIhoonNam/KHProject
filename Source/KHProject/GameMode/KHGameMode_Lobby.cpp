// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KHGameMode_Lobby.h"

#include "KHGameState_Lobby.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"


AKHGameMode_Lobby::AKHGameMode_Lobby()
{
	bUseSeamlessTravel = true;

	PlayerControllerClass = PlayerController;

	PlayerStateClass = PlayerState;	
}

void AKHGameMode_Lobby::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

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
	GetWorld()->ServerTravel(MainGameMapPath, false, false);
}

void AKHGameMode_Lobby::UpdateLobbyStatus()
{

	CheckStartCondition();

}

void AKHGameMode_Lobby::NotifyPlayerReady(const FString& PlayerName)
{
	if (AKHGameState_Lobby* LobbyGameState = GetGameState<AKHGameState_Lobby>())
	{
		LobbyGameState->OnPlayerReady(PlayerName);
	}

	UpdateLobbyStatus();
}

void AKHGameMode_Lobby::CheckStartCondition()
{

	if (AKHGameState_Lobby* LobbyGameState = GetGameState<AKHGameState_Lobby>())
	{
		bool bAllReady = true;
		if (LobbyGameState->arrPlayerNames.IsEmpty()) return;
		for (FLobbyPlayerInfo PlayerInfo : LobbyGameState->arrPlayerNames)
		{
			if (!PlayerInfo.bIsReady)
			{
				bAllReady = false;
				break;
			}
		}


		if (bAllReady)
		{
			GetWorld()->GetTimerManager().SetTimer(
				StartLobbyTimer,
				this,
				&AKHGameMode_Lobby::StartGame,
				5.0f,
				false
			);
		}
		else if (StartLobbyTimer.IsValid())	
		{
			GetWorld()->GetTimerManager().ClearTimer(StartLobbyTimer);
		}
	}
}

