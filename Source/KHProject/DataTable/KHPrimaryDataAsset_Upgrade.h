// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KHPrimaryDataAsset_Upgrade.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHPrimaryDataAsset_Upgrade : public UPrimaryDataAsset
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
