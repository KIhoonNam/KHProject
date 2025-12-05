// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/KHPlayerController_Player.h"

#include "AbilitySystemComponent.h"
#include "KHCharacter_Player.h"
#include "DataTable/KHDataTable_Upgrade.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/KHGameMode_Lobby.h"
#include "GameMode/KHGameState_Lobby.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/KHHUD_Lobby.h"
#include "UI/Widget/KHWidget_Lobby.h"
#include "UI/Widget/KHWidget_Upgrade.h"


void AKHPlayerController_Player::BeginPlay()
{
	Super::BeginPlay();

	if (AKHGameState_Lobby* pLobby = Cast<AKHGameState_Lobby>(GetWorld()->GetGameState()))
	{
		if (AKHHUD_Lobby* pHUD = Cast<AKHHUD_Lobby>(GetHUD()))
		{
			if (UKHWidget_Lobby* pLobbyWidget = Cast<UKHWidget_Lobby>(pHUD->GetHUDWidget()))
			{
				pLobbyWidget->ConnectGameState();
			}
		}
	}
}

void AKHPlayerController_Player::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (APawn* MyPawn = GetPawn())
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		if (HitResult.bBlockingHit)
		{
			FVector MouseLocation = HitResult.Location;
			FVector PawnLocation = MyPawn->GetActorLocation();
			
			MouseLocation.Z = PawnLocation.Z;
			
			
			
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, MouseLocation);

			TargetRotation.Roll = 0.0f;
			TargetRotation.Pitch = 0.0f;
			FRotator NewRotation = FMath::RInterpTo(MyPawn->GetActorRotation(), TargetRotation, DeltaTime, 15.0f);
			
			MyPawn->SetActorRotation(NewRotation);
		}
	}
}


void AKHPlayerController_Player::Client_LevelUpShowUI_Implementation(const TArray<FName>& arrUpgradeDataName)
{
	UE_LOG(LogTemp,Warning,TEXT("GetUpgrade %d"),arrUpgradeDataName.Num())
	m_arrCurrentRowNames = arrUpgradeDataName;
	if (m_UpgradeWidgetClass)
	{
		if (UKHWidget_Upgrade* pUpgrade = CreateWidget<UKHWidget_Upgrade>(this,m_UpgradeWidgetClass))
		{
			pUpgrade->AddToViewport();

			if (m_UpgradeDataTable)
			{
				TArray<FUpgradeData*> arrUpgradeData;

				for (const FName& rowName : arrUpgradeDataName)
				{
					if (FUpgradeData* pData = m_UpgradeDataTable->FindRow<FUpgradeData>(rowName,TEXT("UpgradeData")))
					{
						arrUpgradeData.Add(pData);
					}
				}
				pUpgrade->SetUpgradeList(arrUpgradeData);

				m_pCurrentUpgradeWidget = pUpgrade;
			}
		}
	}
}

void AKHPlayerController_Player::Server_SelectUpgrade_Implementation(const FName& selectedRowName)
{
	if (selectedRowName == NAME_None)
	{
		UE_LOG(LogTemp,Warning,TEXT("RowName is None"))
		return;
	}

	if (m_UpgradeDataTable)
	{
		if (FUpgradeData* upgradeData = m_UpgradeDataTable->FindRow<FUpgradeData>(selectedRowName,TEXT("UpgradeData")))
		{
			if (upgradeData->UpgradeEffect)
			{
				if (AKHCharacter_Player* MyPawn = Cast<AKHCharacter_Player>(GetPawn()))
				{
					UAbilitySystemComponent* ASC = MyPawn->GetAbilitySystemComponent();
					if (ASC)
					{
						FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
						Context.AddSourceObject(MyPawn);
						ASC->ApplyGameplayEffectToSelf(
							upgradeData->UpgradeEffect.GetDefaultObject(), 
							1.0f, 
							Context
						);
					}
				}
			}
		}
	}
}

void AKHPlayerController_Player::OnLevelUpShowUI()
{
	if (m_UpgradeDataTable)
	{
		TArray<FName> m_arrUpgradeData = m_UpgradeDataTable->GetRowNames();

		Client_LevelUpShowUI(m_arrUpgradeData);
	}
}

void AKHPlayerController_Player::SelectUpgradeRow(int32 rowIndex)
{
	if (m_pCurrentUpgradeWidget.IsValid())
	{
		m_pCurrentUpgradeWidget->RemoveFromParent();
		m_pCurrentUpgradeWidget = nullptr;
	}
	Server_SelectUpgrade(GetSelectedRowUpgradeRowName(rowIndex));
}

FName AKHPlayerController_Player::GetSelectedRowUpgradeRowName(int32 index)
{
	FName OutName = NAME_None;

	if (m_arrCurrentRowNames.IsValidIndex(index))
	{
		OutName = m_arrCurrentRowNames[index];
	}
	return OutName;
}

void AKHPlayerController_Player::Server_PlayerReady_Implementation()
{
	if (AKHGameMode_Lobby* pGameMode = Cast<AKHGameMode_Lobby>(GetWorld()->GetAuthGameMode()))
	{
		FString PlayerGuid = PlayerState->GetPlayerName();
		pGameMode->NotifyPlayerReady(PlayerGuid);
	}


}
