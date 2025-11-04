// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KHAnimInstance_Player.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHAnimInstance_Player : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bIsMove;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float fDirection;
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	

	
};
