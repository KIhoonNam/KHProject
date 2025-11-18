// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KHWidget_Main.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidget_Main : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_Join;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickJoin();
	
};
