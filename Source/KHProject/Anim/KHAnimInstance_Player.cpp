// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/KHAnimInstance_Player.h"

#include "AbilitySystemComponent.h"
#include "KHCharacter_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UKHAnimInstance_Player::UKHAnimInstance_Player()
{
	bIsDowned = false;
}

void UKHAnimInstance_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (TryGetPawnOwner())
	{
		if (AKHCharacter_Player* PlayerCharacter = Cast<AKHCharacter_Player>(TryGetPawnOwner()))
		{
			FVector vecVelocity = PlayerCharacter->GetVelocity();
			vecVelocity.Z = 0.0f;
			float Speed = vecVelocity.Size2D();
			bIsMove = Speed > 3.f;
			fDirection = UKismetAnimationLibrary::CalculateDirection(vecVelocity, PlayerCharacter->GetActorRotation());

			bool IsDowned = PlayerCharacter->m_IsCurrentDowned;
			bIsDowned = IsDowned;

			FRotator ControlRotation = PlayerCharacter->GetBaseAimRotation();
			
			FRotator ActorRotation = PlayerCharacter->GetActorRotation();
			
			FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation);

			AimRotation = DeltaRot;
		}
	}
}
