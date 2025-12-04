// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DataTable/KHDataTable_WeaponData.h"
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
	
	UPROPERTY(Replicated,EditDefaultsOnly)
	EWeaponType m_eWeaponType = EWeaponType::Shotgun;
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
public:
	bool IsDown() {return bIsDown;}
	
	void SetIsDowned(bool _enable);public:
	
	
	UFUNCTION()
	void OnRep_IsDowned();

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_WeaponChange(EWeaponType eweapon);
};
