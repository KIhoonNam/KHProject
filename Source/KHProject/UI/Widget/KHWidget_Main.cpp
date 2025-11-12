// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidget_Main.h"

#include "Button.h"
#include "Kismet/GameplayStatics.h"


void UKHWidget_Main::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Join)
	{
		Btn_Join->OnClicked.AddDynamic(this, &UKHWidget_Main::OnClickJoin);
	}
}

void UKHWidget_Main::OnClickJoin()
{
	UE_LOG(LogTemp,Warning,TEXT("OnClickJoin"));
	UGameplayStatics::OpenLevel(GetWorld(),"127.0.0.1");
}
