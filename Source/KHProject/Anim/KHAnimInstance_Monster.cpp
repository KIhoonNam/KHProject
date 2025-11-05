// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/KHAnimInstance_Monster.h"


void UKHAnimInstance_Monster::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (GetOwningActor())
	{
		FVector velocity = GetOwningActor()->GetVelocity();
		velocity.Z = 0.f;

		m_fSpeed = velocity.Size();
	}
}
