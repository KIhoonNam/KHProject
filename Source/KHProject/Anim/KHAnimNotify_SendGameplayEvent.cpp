// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/KHAnimNotify_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"


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

	if (OwnerActor->HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("[Notify] Owner: %s / Mesh: %s / IsServer: %d"), 
				*MeshComp->GetOwner()->GetName(), 
				*MeshComp->GetName(), 
				MeshComp->GetOwner()->HasAuthority());
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			OwnerActor,       
			EventTag,        
			FGameplayEventData() 
		);
	}
}
