// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "KHDataTable_Wave.generated.h"


class AKHCharacter_MonsterBase;

USTRUCT(BlueprintType)
struct FWaveData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WaveData")
	TSubclassOf<AKHCharacter_MonsterBase> EnemyToSpawn; 


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WaveData")
	int32 TotalSpawnCount;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WaveData")
	float TimeBetweenSpawns;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WaveData")
	float RestTimeAfterWave;

	
	
};