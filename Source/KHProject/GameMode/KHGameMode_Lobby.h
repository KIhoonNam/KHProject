// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "KHGameMode_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHGameMode_Lobby : public AGameMode
{
	GENERATED_BODY()
public:
	AKHGameMode_Lobby();

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerState> PlayerState;
	
	UPROPERTY(EditDefaultsOnly)
	FString MainGameMapPath;

	
	FTimerHandle StartLobbyTimer;
public:
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;

	void StartGame();


public:
	void UpdateLobbyStatus();

	void NotifyPlayerReady(const FString& PlayerName);
private:
	void CheckStartCondition();
};
