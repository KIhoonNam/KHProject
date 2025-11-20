// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Component/KHWidgetComponent.h"


void UKHWidgetComponent::InitWidget()
{
	Super::InitWidget();

	if (OnWidgetCreate.IsBound())
	{
		OnWidgetCreate.Execute();
	}
}
