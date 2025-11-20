// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/MyAnimNotify_Reload.h"

#include "KHCharacter_Player.h"


void UMyAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


	if (MeshComp)
	{
		if (AKHCharacter_Player* pOwner = Cast<AKHCharacter_Player>(MeshComp->GetOwner()))
		{
			pOwner->HandleReloadWeapon(m_IsState);
		}
	}
}
