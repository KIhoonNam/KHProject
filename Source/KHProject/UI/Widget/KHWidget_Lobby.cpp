// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidget_Lobby.h"

#include "Button.h"
#include "KHWidgetSlot_Player.h"
#include "TextBlock.h"
#include "VerticalBox.h"
#include "Controller/KHPlayerController_Player.h"
#include "GameMode/KHGameState_Lobby.h"
#include "Kismet/GameplayStatics.h"


void UKHWidget_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	ConnectGameState();

	if (Btn_Ready)
	{
		Btn_Ready->OnClicked.AddDynamic(this,&UKHWidget_Lobby::OnClickReady);
	}
	if (Btn_LogOut)
	{
		Btn_LogOut->OnClicked.AddDynamic(this,&UKHWidget_Lobby::OnClickLogOut);
	}
}

void UKHWidget_Lobby::NativeDestruct()
{
	if (AKHGameState_Lobby* pLobby = Cast<AKHGameState_Lobby>(GetWorld()->GetGameState()))
	{
		pLobby->OnUpdateCurrentPlayers.RemoveAll(this);
	}
	if (Btn_Ready)
	{
		Btn_Ready->OnClicked.RemoveAll(this);
	}
	if (Btn_LogOut)
	{
		Btn_LogOut->OnClicked.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UKHWidget_Lobby::ConnectGameState()
{
	if (AKHGameState_Lobby* pLobby = Cast<AKHGameState_Lobby>(GetWorld()->GetGameState()))
	{
		pLobby->OnUpdateCurrentPlayers.AddUObject(this,&UKHWidget_Lobby::UpdateCurrentPlayers);
	}
}



void UKHWidget_Lobby::UpdateCurrentPlayers(int Num, TArray<FLobbyPlayerInfo> PlayerNames)
{
	if (Text_PlayerNum)
	{
		FString strText = FString::Printf(TEXT("현재 인원: %d"), Num);
		Text_PlayerNum->SetText(FText::FromString(strText));

		if (Vertical_Player)
		{
			Vertical_Player->ClearChildren();


			for (FLobbyPlayerInfo playerName : PlayerNames)
			{

					if (UKHWidgetSlot_Player* pSlot = CreateWidget<UKHWidgetSlot_Player>(this,PlayerSlotClass))
					{
						Vertical_Player->AddChildToVerticalBox(pSlot);
						pSlot->SetSlots(playerName);
					}
				
			}
		}
		
	}
}

void UKHWidget_Lobby::OnClickReady()
{
	if (AKHPlayerController_Player* pController = Cast<AKHPlayerController_Player>(GetOwningPlayer()))
	{
		pController->Server_PlayerReady();
	}
}

void UKHWidget_Lobby::OnClickLogOut()
{
	UGameplayStatics::OpenLevel(GetWorld(),"Level_Main");
}
