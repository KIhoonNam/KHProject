// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KHWidgetSlot_Upgrade.generated.h"

struct FUpgradeData;
class UGameplayEffect;
class UTextBlock;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidgetSlot_Upgrade : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(EditAnywhere)
	int32 SlotIndex;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_Title;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_Description;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_Number;

	UPROPERTY()
	FName RowName;

public:
	virtual void NativeConstruct() override;


public:
	void SetUpgradeData(FUpgradeData* Upgradata);
};
