// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KHPlayerState.h"

#include "KHGameMode_Lobby.h"
#include "KHGameMode_Play.h"
#include "KHGameState_Lobby.h"
#include "UnrealNetwork.h"


void AKHPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AKHPlayerState, bIsDown);
    DOREPLIFETIME(AKHPlayerState, m_eWeaponType);
}

void AKHPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    UE_LOG(LogTemp,Warning,TEXT("AKHPlayerState::CopyProperties"));
    AKHPlayerState* NewPlayerState = Cast<AKHPlayerState>(PlayerState);
    if (NewPlayerState)
    {
        NewPlayerState->m_eWeaponType = this->m_eWeaponType;
    }
}

void AKHPlayerState::SetIsDowned(bool _enable)
{
    if (HasAuthority())
        {
            bIsDown = _enable;
            AKHGameMode_Play* pGameMode = Cast<AKHGameMode_Play>(GetWorld()->GetAuthGameMode());
            if (pGameMode)
            {
                pGameMode->CheckForGameOver();
            }
        }
}

void AKHPlayerState::OnRep_IsDowned()
{

}

void AKHPlayerState::Server_WeaponChange_Implementation(EWeaponType eweapon)
{
    m_eWeaponType = eweapon;
    FString PlayerName = GetPlayerName();
    if (AKHGameState_Lobby* pLobbyState = Cast<AKHGameState_Lobby>(GetWorld()->GetGameState()))
    {
        pLobbyState->OnPlayerWeapon(m_eWeaponType,PlayerName);
    }
    
}

bool AKHPlayerState::Server_WeaponChange_Validate(EWeaponType eweapon)
{
    if (eweapon == EWeaponType::None)
    {
        return false;
    }

    return true;
}
