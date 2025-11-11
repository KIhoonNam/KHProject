// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/KHGameMode_Play.h"

#include "KHPlayerState.h"
#include "GameFramework/GameStateBase.h"


void AKHGameMode_Play::CheckForGameOver()
{
	int32 AlivePlayers = 0;

	UE_LOG(LogTemp, Display, TEXT("CheckForGameOver"));
	// 1. GameState에 있는 모든 PlayerState 배열을 순회합니다.
	if (GameState == nullptr) return;
	
	for (APlayerState* PS : GameState->PlayerArray)
	{
		AKHPlayerState* pPS = Cast<AKHPlayerState>(PS);
		if (pPS)
		{
			if (!pPS->IsDown())
			{
				AlivePlayers++;
			}
		}
	}

	if (AlivePlayers == 0)
	{
		//HandleGameOver();
	}
}
