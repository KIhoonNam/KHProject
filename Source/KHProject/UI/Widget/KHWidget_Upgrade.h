// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KHWidget_Upgrade.generated.h"

struct FUpgradeData;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidget_Upgrade : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UHorizontalBox> HorizontalBox_Upgrade;




public:
	void SetUpgradeList(const TArray<FUpgradeData*>& arrUpgradeData);
	
	
};
