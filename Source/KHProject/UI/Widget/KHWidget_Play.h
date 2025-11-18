// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "KHWidget_Play.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;


/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidget_Play : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_CurrentAmmo;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_MaxAmmo;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_CurrentHP;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_MaxHP;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> Progress_Health;

	UPROPERTY()
	float MaxHealthValue;
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


private:
	void OnCurrentAmmoChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);
};
