// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/KHWidget_Upgrade.h"

#include "HorizontalBox.h"
#include "KHWidgetSlot_Upgrade.h"


void UKHWidget_Upgrade::SetUpgradeList(const TArray<FUpgradeData*>& arrUpgradeData)
{
	if (HorizontalBox_Upgrade)
	{
		int ChildNum = HorizontalBox_Upgrade->GetChildrenCount();

		for (int i = 0; i < ChildNum; ++i)
		{
			
			if (UKHWidgetSlot_Upgrade* ChildWidget = Cast<UKHWidgetSlot_Upgrade>(HorizontalBox_Upgrade->GetChildAt(i)))
			{
				if (arrUpgradeData.IsValidIndex(i))
				{
					ChildWidget->SetUpgradeData(arrUpgradeData[i]);
				}
			}
		}
	}
}
