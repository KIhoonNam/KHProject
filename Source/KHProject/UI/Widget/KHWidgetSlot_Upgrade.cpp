// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidgetSlot_Upgrade.h"

#include "TextBlock.h"
#include "DataTable/KHDataTable_Upgrade.h"


void UKHWidgetSlot_Upgrade::NativeConstruct()
{
	Super::NativeConstruct();

	if (Text_Number)
	{
		Text_Number->SetText(FText::AsNumber(SlotIndex));
	}
}

void UKHWidgetSlot_Upgrade::SetUpgradeData(FUpgradeData* Upgradata)
{
	if (Upgradata)
	{
		if (Text_Description)
		{
			Text_Description->SetText(Upgradata->Text_Description);
		}

		if (Text_Title)
		{
			Text_Title->SetText(Upgradata->Text_Title);
		}
	}
}
