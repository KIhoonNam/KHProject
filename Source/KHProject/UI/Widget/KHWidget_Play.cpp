// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidget_Play.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "KHAttributeSet_Character.h"
#include "ProgressBar.h"
#include "TextBlock.h"


void UKHWidget_Play::NativeConstruct()
{
	Super::NativeConstruct();

	AActor* OwningPlayer = GetOwningPlayerPawn();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPlayer);
	
	if (ASC)
	{
		const UKHAttributeSet_Character* Attributes = ASC->GetSet<UKHAttributeSet_Character>();
		if (Attributes)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attributes->GetCurrentAmmoAttribute()).AddUObject(this, &UKHWidget_Play::OnCurrentAmmoChanged);
			
			ASC->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this, &UKHWidget_Play::OnHealthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxHealthAttribute()).AddUObject(this, &UKHWidget_Play::OnMaxHealthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(Attributes->GetXPAttribute()).AddUObject(this, &UKHWidget_Play::OnXPChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxXPAttribute()).AddUObject(this, &UKHWidget_Play::OnMaxXPChange);
			ASC->GetGameplayAttributeValueChangeDelegate(Attributes->GetCharacterLevelAttribute()).AddUObject(this, &UKHWidget_Play::OnLevelChange);
			
			if (Text_CurrentAmmo)
			{
				Text_CurrentAmmo->SetText(FText::AsNumber(Attributes->GetCurrentAmmo()));
			}
			if (Text_CurrentHP)
			{
				Text_CurrentHP->SetText(FText::AsNumber(Attributes->GetHealth()));
			}
			if (Text_MaxHP)
			{
				Text_MaxHP->SetText(FText::AsNumber(Attributes->GetMaxHealth()));
				MaxHealthValue = Attributes->GetMaxHealth();
			}
			if (Progress_Health)
			{
				Progress_Health->SetPercent(Attributes->GetHealth() / Attributes->GetMaxHealth());
			}
			float XP =Attributes->GetXP();
			MaxXP = Attributes->GetMaxXP();
			if (Text_CurrentXP)
			{
				Text_CurrentXP->SetText(FText::AsNumber(XP));
			}
			if (Text_MaxXP)
			{
				Text_MaxXP->SetText(FText::AsNumber(MaxXP));
			}
			if (Text_Level)
			{
				Text_Level->SetText(FText::AsNumber(Attributes->GetCharacterLevel()));
			}
			if (Progress_XP)
			{
				Progress_XP->SetPercent(XP / MaxXP);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Found AbilitySystemComponent in KHWidget_Play"));
	}
}

void UKHWidget_Play::NativeDestruct()
{


	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayer());
	
	if (ASC)
	{
		const UKHAttributeSet_Character* Attributes = ASC->GetSet<UKHAttributeSet_Character>();
		if (Attributes)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attributes->GetCurrentAmmoAttribute()).RemoveAll(this);
			
			ASC->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).RemoveAll(this);
		}
	}

	Super::NativeDestruct();
}

void UKHWidget_Play::OnCurrentAmmoChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (Text_CurrentAmmo)
	{
		Text_CurrentAmmo->SetText(FText::AsNumber(OnAttributeChangeData.NewValue));
	}
}


void UKHWidget_Play::OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (Progress_Health)
	{
		Progress_Health->SetPercent(OnAttributeChangeData.NewValue / MaxHealthValue);
	}

	if (Text_CurrentHP)
	{
		Text_CurrentHP->SetText(FText::AsNumber(OnAttributeChangeData.NewValue));
	}
}

void UKHWidget_Play::OnXPChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (Text_CurrentXP)
	{
		Text_CurrentXP->SetText(FText::AsNumber(OnAttributeChangeData.NewValue));
	}

	if (Progress_XP)
	{
		Progress_XP->SetPercent(OnAttributeChangeData.NewValue/MaxXP);
	}
}

void UKHWidget_Play::OnMaxXPChange(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (Text_MaxXP)
	{
		Text_MaxXP->SetText(FText::AsNumber(OnAttributeChangeData.NewValue));
	}

	MaxXP =OnAttributeChangeData.NewValue;
}

void UKHWidget_Play::OnLevelChange(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (Text_Level)
	{
		Text_Level->SetText(FText::AsNumber(OnAttributeChangeData.NewValue));
	}
}

void UKHWidget_Play::OnMaxHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (Text_MaxHP)
	{
		Text_MaxHP->SetText(FText::AsNumber(OnAttributeChangeData.NewValue));
		MaxHealthValue = OnAttributeChangeData.NewValue;
	}
}
