// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KHAnimInstance_Monster.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHAnimInstance_Monster : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	float m_fSpeed;
	
};
