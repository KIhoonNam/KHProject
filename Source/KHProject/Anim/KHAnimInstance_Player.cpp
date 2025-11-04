// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/KHAnimInstance_Player.h"

#include "KHCharacter_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UKHAnimInstance_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (GetOwningActor())
	{
		if (AKHCharacter_Player* PlayerCharacter = Cast<AKHCharacter_Player>(GetOwningActor()))
		{
			FVector vecVelocity = PlayerCharacter->GetVelocity();
			vecVelocity.Z = 0.0f;
			float Speed = vecVelocity.Size2D();
			bIsMove = Speed > 3.f;
			fDirection = UKismetAnimationLibrary::CalculateDirection(vecVelocity, PlayerCharacter->GetActorRotation());
		}
	}
}
