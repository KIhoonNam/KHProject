// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "KHWidgetComponent.generated.h"


DECLARE_DELEGATE(FOnWidgetCreate);
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	FOnWidgetCreate OnWidgetCreate;

public:
	virtual void InitWidget() override;
	
};
