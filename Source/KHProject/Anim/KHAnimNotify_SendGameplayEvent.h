// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "KHAnimNotify_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()


public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FGameplayTag EventTag;
	
};
