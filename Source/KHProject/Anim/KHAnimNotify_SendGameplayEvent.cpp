// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/KHAnimNotify_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"


void UKHAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
	{
		return;
	}


	AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}


	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwnerActor,       
		EventTag,        
		FGameplayEventData() 
	);
}
