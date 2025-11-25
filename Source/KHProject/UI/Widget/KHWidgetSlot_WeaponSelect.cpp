// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidgetSlot_WeaponSelect.h"
#include "Character/KHCharacter_Player.h"
#include "Button.h"
#include "Image.h"
#include "TextBlock.h"
#include "GameMode/KHPlayerState.h"


void UKHWidgetSlot_WeaponSelect::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Weapon)
	{
		Button_Weapon->OnClicked.AddDynamic(this,&UKHWidgetSlot_WeaponSelect::OnClickWeapon);
	}

	if (Text_Weapon)
	{						
		FString strWeapon = EnumToString(m_eType);
		Text_Weapon->SetText(FText::FromString(strWeapon));
	}

	if (Image_Weapon)
	{
		Image_Weapon->SetBrush(m_WeaponBrush);
	}
}

void UKHWidgetSlot_WeaponSelect::NativeDestruct()
{
	if (Button_Weapon)
	{
		Button_Weapon->OnClicked.RemoveAll(this);
	}
	
	Super::NativeDestruct();
}

void UKHWidgetSlot_WeaponSelect::OnClickWeapon()
{
	if (AKHPlayerState* pState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AKHPlayerState>())
	{
		pState->Server_WeaponChange(m_eType);
	}
}
