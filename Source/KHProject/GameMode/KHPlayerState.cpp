// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KHPlayerState.h"

#include "KHGameMode_Play.h"
#include "UnrealNetwork.h"


void AKHPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AKHPlayerState, bIsDown);
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
