// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/KHGameState_Lobby.h"
#include "KHWidget_Lobby.generated.h"


class UTextBlock;
class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidget_Lobby : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayerSlotClass;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Text_PlayerNum;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* Btn_Ready;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* Btn_LogOut;
	
	UPROPERTY(meta=(BindWidgetOptional))
	UVerticalBox* Vertical_Player;


	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


	void ConnectGameState();
	

private:
	void UpdateCurrentPlayers(int Num, TArray<FLobbyPlayerInfo> PlayerNames);

	UFUNCTION()
	void OnClickReady();

	UFUNCTION()
	void OnClickLogOut();
};
