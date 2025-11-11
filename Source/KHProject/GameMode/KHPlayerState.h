// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_IsDowned)
	bool bIsDown;
	

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	bool IsDown() {return bIsDown;}
	
	void SetIsDowned(bool _enable);public:
	
	
	UFUNCTION()
	void OnRep_IsDowned();
};
