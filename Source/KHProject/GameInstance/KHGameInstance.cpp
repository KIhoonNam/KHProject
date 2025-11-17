// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/KHGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionNames.h"

UKHGameInstance::UKHGameInstance()
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UKHGameInstance::OnCreateSessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UKHGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UKHGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UKHGameInstance::OnDestroySessionComplete);

}

void UKHGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
		SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
		UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
	}


	// if (IsDedicatedServerInstance())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Dedicated Server detected. Hosting session from Lobby..."));
	//
	// 	HostSession(3); 
	// }

	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
	}
	
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UKHGameInstance::OnWorldCreated);
}

void UKHGameInstance::FindAndJoinSession(const FString& IPAddress)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SessionInterface is not valid!"));
		return;
	}
	SearchResults.Empty();
	UE_LOG(LogTemp, Log, TEXT("CLIENT: Finding sessions..."));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->bIsLanQuery = true;
	SessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SEARCH_EMPTY_SERVERS_ONLY, false, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SEARCH_MINSLOTSAVAILABLE, 0, EOnlineComparisonOp::Equals);
	// 검색 시작
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UKHGameInstance::JoinFoundSession(int32 SessionIndex)
{
	if (!SessionInterface.IsValid() || !SearchResults.IsValidIndex(SessionIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to join session: Invalid data."));
		return;
	}

	const FOnlineSessionSearchResult& SearchResult = SearchResults[SessionIndex];

	// ★★★ 바로 여기가 기훈님이 원하셨던 인원수 체크 로직입니다 ★★★
	const int32 MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
	const int32 CurrentPlayers = MaxPlayers - SearchResult.Session.NumOpenPublicConnections;

	if (CurrentPlayers >= MaxPlayers-1)
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Server is full! (%d/%d)"), CurrentPlayers, MaxPlayers);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("CLIENT: Joining session... (%d/%d)"), CurrentPlayers, MaxPlayers);

	
	// 세션 참가 요청
	SessionInterface->JoinSession(0, NAME_GameSession, SearchResult);
}

void UKHGameInstance::CreateNewSession()
{
	if (!SessionInterface.IsValid()) return;

	UE_LOG(LogTemp, Log, TEXT("Creating new session..."));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPublicConnections = 4; // 임시 저장된 변수 사용
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bIsDedicated = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.Set(SETTING_MAPNAME, FString("Level_Lobby"), EOnlineDataAdvertisementType::ViaOnlineService);

	if (!SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings))
	{
		UE_LOG(LogTemp, Error, TEXT("SERVER: CreateSession call failed immediately."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SERVER: CreateSession call successful. Waiting for delegate..."));
	}
}

void UKHGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type Arg,
                                           const FString& String)
{
	
}

void UKHGameInstance::HostSession(int I)
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SessionInterface is NOT VALID. Check .ini and .Build.cs files!"));
		return;
	}
	

	// 'GameSession'이라는 이름의 세션이 이미 있는지 확인
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
    
	if (ExistingSession != nullptr)
	{
		// ★ 이미 있다면: 먼저 파괴
		UE_LOG(LogTemp, Warning, TEXT("Found existing session. Destroying it first..."));
        
		// DestroySession이 완료되면 OnDestroySessionComplete 델리게이트가 호출됨
		if (!SessionInterface->DestroySession(NAME_GameSession, OnDestroySessionCompleteDelegate))
		{
			UE_LOG(LogTemp, Error, TEXT("SERVER: DestroySession call failed immediately."));
		}
	}
	else
	{
		// ★ 없다면: 바로 생성
		UE_LOG(LogTemp, Log, TEXT("No existing session found. Creating new one..."));
		CreateNewSession();
	}
}

void UKHGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{

	if (bWasSuccessful && SessionSearch.IsValid())
	{
		SearchResults = SessionSearch->SearchResults;
		JoinFoundSession(0);
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Found %d sessions."), SearchResults.Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CLIENT: Failed to find sessions."));
		SearchResults.Empty();
	}
}

void UKHGameInstance::OnDestroySessionComplete(FName Name, bool bArg)
{
	if (bArg)
	{
		UE_LOG(LogTemp, Log, TEXT("SERVER: Old session destroyed. Setting timer to create new one..."));
		
		UWorld* World = GetWorld(); 
		if (World)
		{
			FTimerHandle TimerHandle_CreateSession;
			World->GetTimerManager().SetTimer(
				TimerHandle_CreateSession, 
				this, 
				&UKHGameInstance::CreateNewSession, 
				0.1f,  
				false  
			);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SERVER: No World found, creating session immediately. (May fail)"));
			CreateNewSession();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SERVER: Failed to destroy old session."));
	}
}

void UKHGameInstance::OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Arg)
{
	if (Arg != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("CLIENT: Failed to join session. Result: %d"), (int32)Arg);
		return;
	}
	
	FString ConnectString;
	if (SessionInterface->GetResolvedConnectString(Name, ConnectString))
	{
		UE_LOG(LogTemp, Warning, TEXT("CLIENT: Join successful. Traveling to: %s"), *ConnectString);
		
		APlayerController* PC = GetFirstLocalPlayerController();
		if (PC)
		{
			PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CLIENT: Could not get connect string."));
	}
}

void UKHGameInstance::CheckSessionCapacity(const FOnlineSessionSearchResult& SearchResult)
{
}

void UKHGameInstance::OnWorldCreated(UWorld* World, FWorldInitializationValues WorldInitializationValues)
{

	if (World == nullptr  || World->GetGameInstance() != this)
	{
		UE_LOG(LogTemp,Warning,TEXT("noGameInstance"));
		return;
	}


	if (IsDedicatedServerInstance()) 
	{
		UE_LOG(LogTemp,Warning,TEXT("DedicatedServerInstance"));
		HostSession(8);
	}
	else
	{
			if (SessionInterface.IsValid())
			{

				FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
				if (ExistingSession != nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("CLIENT: Found stale session. Destroying it..."));
					SessionInterface->DestroySession(NAME_GameSession);
				}
			}
		
	}
}

void UKHGameInstance::OnCreateSessionComplete(FName Name, bool bArg)
{
	if (bArg)
	{
		UE_LOG(LogTemp, Warning, TEXT("SERVER: Session '%s' created successfully."), *Name.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SERVER: Failed to create session."));
	}
}
