// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "KHDataTable_Upgrade.generated.h"


class UGameplayEffect;
USTRUCT(BlueprintType)
struct FUpgradeData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Upgrade")
	FText Text_Title;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Upgrade")
	FText  Text_Description;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Upgrade")
	TSubclassOf<UGameplayEffect> UpgradeEffect;

	
	
};