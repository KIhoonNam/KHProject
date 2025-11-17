// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidget_Main.h"

#include "Button.h"
#include "GameInstance/KHGameInstance.h"
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
	if (UKHGameInstance* pInstance = Cast<UKHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		pInstance->FindAndJoinSession("IPAddress");
	}
}
