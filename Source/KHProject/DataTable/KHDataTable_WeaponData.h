// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "KHDataTable_WeaponData.generated.h"


class UAnimMontage;
class UStaticMesh;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None        UMETA(DisplayName = "None"),
	Rifle     UMETA(DisplayName = "Rifle"),
	Gun      UMETA(DisplayName = "Gun"),
	Shotgun        UMETA(DisplayName = "Shotgun"),
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool m_IsAuto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float m_fDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float m_fCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_Bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpreadAngle = 0.0f;
};