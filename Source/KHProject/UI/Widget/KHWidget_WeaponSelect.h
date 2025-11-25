// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KHWidget_WeaponSelect.generated.h"


class UScrollBox;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidget_WeaponSelect : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> ScrollBox_Weapon;
	
};
