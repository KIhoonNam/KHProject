// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MyAnimNotify_Reload.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UMyAnimNotify_Reload : public UAnimNotify
{
	GENERATED_BODY()
	
	


public:
	UPROPERTY(EditAnywhere)
	bool m_IsState;


	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
