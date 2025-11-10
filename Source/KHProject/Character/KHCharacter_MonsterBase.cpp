// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KHCharacter_MonsterBase.h"

#include "AbilitySystemComponent.h"
#include "KHAttributeSet_Character.h"


float AKHCharacter_MonsterBase::GetHealth() const
{
	if (m_pAttributeSet)
	{
		return m_pAttributeSet->GetHealth();
	}
	return 0.0f;
}


void AKHCharacter_MonsterBase::Multicast_PlayMeleeAttackMontage_Implementation(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay && GetMesh())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(MontageToPlay);
		}
	}
}

void AKHCharacter_MonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		
		
		if (HasAuthority() )
		{
			if (BaseStatsEffect)
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(BaseStatsEffect, 1.0f, EffectContext);

				if (SpecHandle.IsValid())
				{
					AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
				
				AbilitySystemComponent->ApplyModToAttribute(
					m_pAttributeSet->GetHealthAttribute(), 
					EGameplayModOp::Override, 
					m_pAttributeSet->GetMaxHealth()
				);
			}
			if (m_pAIMeleeAbility)
			{
				FGameplayAbilitySpec MeleeAttackSpec(m_pAIMeleeAbility, 1.0f, -1, this);
				AbilitySystemComponent->GiveAbility(MeleeAttackSpec);
			}
		}
	}
}

void AKHCharacter_MonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(GetController(), this);
}
