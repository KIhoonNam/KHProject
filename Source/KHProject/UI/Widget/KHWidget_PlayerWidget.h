// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "KHWidget_PlayerWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidget_PlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> HP_ProgressBar;


	UPROPERTY()
	float m_fMaxHealth;
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


public:
	void SetupWidget(UAbilitySystemComponent* AbilitySystemComponent);
	
private:
	void OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData);

	
};
