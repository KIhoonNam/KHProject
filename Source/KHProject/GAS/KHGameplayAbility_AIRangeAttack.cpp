// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/KHGameplayAbility_AIRangeAttack.h"

#include "AbilityTask_WaitDelay.h"
#include "AbilityTask_WaitGameplayEvent.h"
#include "KHCharacter_MonsterBase.h"
#include "Actor/KHActor_Projectlie.h"
#include "GameFramework/ProjectileMovementComponent.h"


UKHGameplayAbility_AIRangeAttack::UKHGameplayAbility_AIRangeAttack()
{
	HitCheckEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Attack.Range"));
}

void UKHGameplayAbility_AIRangeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthority(&ActivationInfo) || AttackMontage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AKHCharacter_MonsterBase* AICharacter = Cast<AKHCharacter_MonsterBase>(ActorInfo->AvatarActor.Get());
	if (!AICharacter)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AICharacter->Multicast_PlayMeleeAttackMontage(AttackMontage);


	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,               
		HitCheckEventTag,   
		nullptr,            
		true                
	);

	if (EventTask == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}


	EventTask->EventReceived.AddDynamic(this, &UKHGameplayAbility_AIRangeAttack::OnHitCheckEvent);

	EventTask->ReadyForActivation();

	const float MontageDuration = AttackMontage->GetPlayLength();
    
	
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, MontageDuration);
	if (DelayTask)
	{
		DelayTask->OnFinish.AddDynamic(this, &UKHGameplayAbility_AIRangeAttack::OnMontageEnded);
		DelayTask->ReadyForActivation();
	}
}

void UKHGameplayAbility_AIRangeAttack::OnHitCheckEvent(FGameplayEventData Payload)
{
	if (AKHCharacter_MonsterBase* pOwner = Cast<AKHCharacter_MonsterBase>(GetCurrentActorInfo()->AvatarActor.Get()))
	{
		// You can add any logic here if needed, e.g., logging or effects
	
		if (AKHActor_Projectlie* pProjectile = GetWorld()->SpawnActor<AKHActor_Projectlie>(ProjectileClass,
			pOwner->GetWeaponAttackLocation(),
			GetCurrentActorInfo()->AvatarActor->GetActorRotation()))
		{
			FVector LaunchDirection = GetCurrentActorInfo()->AvatarActor->GetActorForwardVector();
			pProjectile->ProjectlieMovementComponent->Velocity = LaunchDirection * pProjectile->ProjectlieMovementComponent->InitialSpeed;

			if (DamageEffectClass)
			{
				pProjectile->DamageEffectClass = DamageEffectClass;
			}
		}
	}
	
}

void UKHGameplayAbility_AIRangeAttack::OnMontageEnded()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
