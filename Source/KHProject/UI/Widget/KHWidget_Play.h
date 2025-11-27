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

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_CurrentXP;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_MaxXP;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_Level;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> Progress_XP;

	UPROPERTY()
	float MaxHealthValue;

	UPROPERTY()
	float MaxXP;
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


private:
	void OnCurrentAmmoChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnXPChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnMaxXPChange(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnLevelChange(const FOnAttributeChangeData& OnAttributeChangeData);
	void OnMaxHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);
};
