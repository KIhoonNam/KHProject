// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "KHGameInstance.generated.h"

class IOnlineSession;
class FOnlineSessionSearchResult;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UKHGameInstance();
public:
	FTimerHandle NetworkFailedTimerHandle;

	
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	
	TArray<FOnlineSessionSearchResult> SearchResults;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

public:

	virtual void Init() override;

	void FindAndJoinSession(const FString& IPAddress);
	void JoinFoundSession(int32 SessionIndex);
	void CreateNewSession();
	void HostSession(int I);

private:
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type Arg, const FString& String);
	

private:
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnDestroySessionComplete(FName Name, bool bArg);
	void OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Arg);
	void CheckSessionCapacity(const FOnlineSessionSearchResult& SearchResult);
	void OnWorldCreated(UWorld* World, FWorldInitializationValues WorldInitializationValues);

	void OnCreateSessionComplete(FName Name, bool bArg);
	TSharedPtr<IOnlineSession, ESPMode::ThreadSafe> SessionInterface;
};
