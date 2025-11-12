// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KHHUDBase.h"

#include "UserWidget.h"


void AKHHUDBase::BeginPlay()
{
	Super::BeginPlay();

	if (m_HUDWidgetClass)
	{
		if (UUserWidget* HUDWidget = CreateWidget<UUserWidget>(GetWorld(), m_HUDWidgetClass))
		{
			HUDWidget->AddToViewport();
			m_HUDWidget = HUDWidget;
		}
	}
}

UUserWidget* AKHHUDBase::GetHUDWidget()
{
	if (m_HUDWidget)
	{
		return m_HUDWidget;
	}
	return nullptr;
}
