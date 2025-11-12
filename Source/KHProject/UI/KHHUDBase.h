// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KHHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> m_HUDWidgetClass;

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> m_HUDWidget;
public:
	virtual void BeginPlay() override;

public:
	UUserWidget* GetHUDWidget();
};
