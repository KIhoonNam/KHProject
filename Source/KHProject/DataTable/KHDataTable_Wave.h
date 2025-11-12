// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KHCharacter_MonsterBase.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "KHDataTable_Wave.generated.h"




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