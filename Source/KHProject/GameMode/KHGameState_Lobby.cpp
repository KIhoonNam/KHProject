// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KHGameState_Lobby.h"

#include "UnrealNetwork.h"


void AKHGameState_Lobby::OnRep_CurrentPlayers()
{
	if (OnUpdateCurrentPlayers.IsBound())
	{
		OnUpdateCurrentPlayers.Broadcast(CurrentPlayers, arrPlayerNames);
	}
}

void AKHGameState_Lobby::OnPlayerReady(const FString& PlayerName)
{
	FLobbyPlayerInfo* PlayerInfoToModify = arrPlayerNames.FindByPredicate(
	[PlayerName](const FLobbyPlayerInfo& Info) // 람다 함수 시작
	{
		return Info.PlayerName == PlayerName;
	} // 람다 함수 끝
);
	if (PlayerInfoToModify)
	{
		bool _IsReady = PlayerInfoToModify->bIsReady ;
		PlayerInfoToModify->bIsReady = !_IsReady;

		OnRep_CurrentPlayers();
	}
}

void AKHGameState_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKHGameState_Lobby, CurrentPlayers);
	DOREPLIFETIME(AKHGameState_Lobby, arrPlayerNames);
}
