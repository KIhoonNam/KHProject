// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/KHDataTable_Wave.h"
#include "GameFramework/GameMode.h"
#include "KHGameMode_Play.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHGameMode_Play : public AGameMode
{
	GENERATED_BODY()

	AKHGameMode_Play();
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerState> PlayerState;

public:

	FTimerHandle SpawnDelayTimerHandle;

	FTimerHandle RestDelayTimerHandle;
	
	UPROPERTY()
	int32 m_CurrentWave;

	UPROPERTY()
	int32 m_SpawnedMonsterCount;

	UPROPERTY()
	int32 m_AlliveMonsterCount;
	
	FWaveData* m_WaveData;

	UPROPERTY(EditAnywhere)
	UDataTable* m_WaveDataTable;
public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	void StartGame();

	void NextWave();

	void SpawnMonster();

	void OnMonsterKilled();
	
	void GameOver();
	
public:
	void CheckForGameOver();


public:
	UPROPERTY()
	float m_fPlayerCheckTimer;
};
