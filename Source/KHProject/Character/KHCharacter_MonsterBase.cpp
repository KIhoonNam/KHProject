// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KHCharacter_MonsterBase.h"

#include "AbilitySystemComponent.h"
#include "KHAttributeSet_Character.h"


AKHCharacter_MonsterBase::AKHCharacter_MonsterBase()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UE_LOG(LogTemp, Warning, TEXT("Character Constructor Called for: %s"), *GetName());
}

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


}

void AKHCharacter_MonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		
		
		if (HasAuthority() )
		{
			AbilitySystemComponent->InitAbilityActorInfo(this, this);
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

			const UKHAttributeSet_Character* pAttributes  = AbilitySystemComponent->GetSet<UKHAttributeSet_Character>();
			if (pAttributes)
			{
				AbilitySystemComponent->ApplyModToAttribute(pAttributes->GetHealthAttribute(), EGameplayModOp::Override, pAttributes->GetMaxHealth());
			}
		}
	}


}
