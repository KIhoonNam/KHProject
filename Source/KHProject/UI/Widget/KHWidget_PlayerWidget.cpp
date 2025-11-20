// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidget_PlayerWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "KHAttributeSet_Character.h"
#include "ProgressBar.h"


void UKHWidget_PlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UKHWidget_PlayerWidget::NativeDestruct()
{
	
	Super::NativeDestruct();
}

void UKHWidget_PlayerWidget::SetupWidget(UAbilitySystemComponent* AbilitySystemComponent)
{


	UE_LOG(LogTemp,Warning,TEXT("WidgetSet1"))
	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("WidgetSet2"))
		const UKHAttributeSet_Character* Attributes = AbilitySystemComponent->GetSet<UKHAttributeSet_Character>();
		if (Attributes)
		{
			UE_LOG(LogTemp,Warning,TEXT("WidgetSet3"))
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this, &UKHWidget_PlayerWidget::OnHealthChanged);

			m_fMaxHealth = Attributes->GetMaxHealth();

			if (HP_ProgressBar)
			{
				HP_ProgressBar->SetPercent(Attributes->GetHealth() / Attributes->GetMaxHealth());
			}
		}
	}
}

void UKHWidget_PlayerWidget::OnHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (HP_ProgressBar)
	{
		HP_ProgressBar->SetPercent(OnAttributeChangeData.NewValue / m_fMaxHealth);
	}
}
