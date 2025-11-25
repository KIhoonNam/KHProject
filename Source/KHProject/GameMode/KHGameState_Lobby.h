// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/KHDataTable_WeaponData.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "KHGameState_Lobby.generated.h"
USTRUCT(BlueprintType)
struct FLobbyPlayerInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bIsReady;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	EWeaponType m_eWeaponType;
	
	FLobbyPlayerInfo()
	{
		PlayerName = "";
		bIsReady = false;
		m_eWeaponType =EWeaponType::None;
	}
	
	bool operator==(const FLobbyPlayerInfo& Other) const
	{
		return PlayerName == Other.PlayerName;
	}
};


DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdateCurrentPlayersDelegate,int32, const TArray<FLobbyPlayerInfo>);
/**
 * 
 */

UCLASS()
class KHPROJECT_API AKHGameState_Lobby : public AGameState
{
	GENERATED_BODY()

public:
	FUpdateCurrentPlayersDelegate OnUpdateCurrentPlayers;
public:
	UPROPERTY(Replicated)
	int32 CurrentPlayers;

	UPROPERTY(Replicated,ReplicatedUsing=OnRep_CurrentPlayers)
	TArray<FLobbyPlayerInfo> arrPlayerNames;
public:
	UFUNCTION()
	void OnRep_CurrentPlayers();

	void OnPlayerReady(const FString& PlayerName);
	void OnPlayerWeapon(EWeaponType _eWeaponType, const FString& PlayerName);
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
