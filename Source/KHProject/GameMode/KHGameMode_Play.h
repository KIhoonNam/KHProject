// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/KHDataTable_Wave.h"
#include "GameFramework/GameMode.h"
#include "KHGameMode_Play.generated.h"

class AKHActor_Spawner;
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

	UPROPERTY()
	TArray<TObjectPtr<AKHActor_Spawner>> m_arrSpawners;
public:

	FTimerHandle SpawnDelayTimerHandle;

	FTimerHandle RestDelayTimerHandle;

	FTimerHandle StartDelayTimerHandle;
	
	UPROPERTY()
	int32 m_CurrentWave;

	UPROPERTY()
	int32 m_SpawnedMonsterCount;

	UPROPERTY()
	int32 m_AlliveMonsterCount;
	
	FWaveData* m_WaveData;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> m_WaveDataTable;
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

	AKHActor_Spawner* RandomSpawner();
public:
	UPROPERTY()
	float m_fPlayerCheckTimer;
};
