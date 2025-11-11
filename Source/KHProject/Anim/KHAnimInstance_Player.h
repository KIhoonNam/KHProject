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
	UKHAnimInstance_Player();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bIsMove;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float fDirection;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bIsDowned = false;
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	void SetIsDowned(bool _bIsDown) {bIsDowned = _bIsDown;};

	
};
