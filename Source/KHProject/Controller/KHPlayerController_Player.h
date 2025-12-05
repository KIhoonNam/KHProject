// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KHPlayerController_Player.generated.h"

struct FUpgradeData;
class UKHWidget_Upgrade;
class UDataTable;
/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHPlayerController_Player : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="DataTable")
	TObjectPtr<UDataTable> m_UpgradeDataTable;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKHWidget_Upgrade> m_UpgradeWidgetClass;

	UPROPERTY()
	TArray<FName> m_arrCurrentRowNames;

	UPROPERTY()
	TWeakObjectPtr<UKHWidget_Upgrade> m_pCurrentUpgradeWidget;
public:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;


public:
	UFUNCTION(Server,Reliable)
	void Server_PlayerReady();

	UFUNCTION(Client,Reliable)
	void Client_LevelUpShowUI(const TArray<FName>& arrUpgradeData);

	UFUNCTION(Server,Reliable)
	void Server_SelectUpgrade(const FName& selectedRowName);
	
	void OnLevelUpShowUI();

	void SelectUpgradeRow(int32 rowIndex);
	FName GetSelectedRowUpgradeRowName(int32 index);
};
