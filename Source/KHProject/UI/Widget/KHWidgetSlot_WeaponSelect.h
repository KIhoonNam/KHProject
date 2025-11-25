// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTable/KHDataTable_WeaponData.h"
#include "KHWidgetSlot_WeaponSelect.generated.h"


class UButton;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHWidgetSlot_WeaponSelect : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EWeaponType m_eType;

	UPROPERTY(EditAnywhere,Category = Weapon)
	FSlateBrush m_WeaponBrush;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Button_Weapon;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UImage> Image_Weapon;

	UPROPERTY(meta =(BindWidgetOptional))
	TObjectPtr<UTextBlock> Text_Weapon;


public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnClickWeapon();
};
