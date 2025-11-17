// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KHGameMode_Play.h"

#include "KHPlayerState.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"


AKHGameMode_Play::AKHGameMode_Play()
{
	m_CurrentWave = 0;
	m_SpawnedMonsterCount = 0;
	m_AlliveMonsterCount = 0;

	bUseSeamlessTravel = true;

	PlayerControllerClass = PlayerController;

	PlayerStateClass = PlayerState;	
}

void AKHGameMode_Play::BeginPlay()
{
	Super::BeginPlay();
}

void AKHGameMode_Play::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
		if (GetGameState<AGameState>()->PlayerArray.Num()<=0)
		{
			m_fPlayerCheckTimer += DeltaSeconds;
			if (m_fPlayerCheckTimer >= 5.0f)
			{
				GetWorld()->ServerTravel(TEXT("Level_Lobby"));
				m_fPlayerCheckTimer = 0.0f;
			}
		}
}

void AKHGameMode_Play::StartGame()
{
}

void AKHGameMode_Play::NextWave()
{
	const FName RowName = FName(*FString::FromInt(m_CurrentWave));
	m_WaveData = m_WaveDataTable->FindRow<FWaveData>(RowName, TEXT("FindWaveData"));


	if (m_WaveData)
	{
		m_SpawnedMonsterCount = m_WaveData->TotalSpawnCount;
		
	
		GetWorldTimerManager().SetTimer(
			SpawnDelayTimerHandle,
			this,
			&AKHGameMode_Play::SpawnMonster,
			m_WaveData->TimeBetweenSpawns,
			true, 
			0.0f  
		);
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("=== 모든 웨이브 클리어! 게임 승리! ==="));
	}
}

void AKHGameMode_Play::SpawnMonster()
{
	if (m_SpawnedMonsterCount <= 0)
	{
		GetWorldTimerManager().ClearTimer(SpawnDelayTimerHandle);
		return;
	}

	if (m_WaveData == nullptr) return;
	if (AKHCharacter_MonsterBase* pMonster = GetWorld()->SpawnActor<AKHCharacter_MonsterBase>(m_WaveData->EnemyToSpawn))
	{
		
	}

	m_SpawnedMonsterCount--;
	m_AlliveMonsterCount++;
}

void AKHGameMode_Play::OnMonsterKilled()
{
	if (m_AlliveMonsterCount > 0)
	{
		m_AlliveMonsterCount--;
	}
	


	if (m_AlliveMonsterCount <= 0 && m_SpawnedMonsterCount <= 0)
	{
		if (m_WaveData)
		{;

			GetWorldTimerManager().SetTimer(
				RestDelayTimerHandle,
				this,
				&AKHGameMode_Play::NextWave,
				m_WaveData->RestTimeAfterWave,
				false
			);
		}
	}
}

void AKHGameMode_Play::GameOver()
{
	
}

void AKHGameMode_Play::CheckForGameOver()
{
	int32 AlivePlayers = 0;

	UE_LOG(LogTemp, Display, TEXT("CheckForGameOver"));
	// 1. GameState에 있는 모든 PlayerState 배열을 순회합니다.
	if (GameState == nullptr) return;
	
	for (APlayerState* PS : GameState->PlayerArray)
	{
		AKHPlayerState* pPS = Cast<AKHPlayerState>(PS);
		if (pPS)
		{
			if (!pPS->IsDown())
			{
				AlivePlayers++;
			}
		}
	}

	if (AlivePlayers == 0)
	{
		//HandleGameOver();
	}
}
