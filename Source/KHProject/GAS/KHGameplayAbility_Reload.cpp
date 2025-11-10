// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/KHGameplayAbility_Reload.h"

#include "AbilityTask_WaitDelay.h"
#include "KHAttributeSet_Character.h"


UKHGameplayAbility_Reload::UKHGameplayAbility_Reload()
{


	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

bool UKHGameplayAbility_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	const UAbilitySystemComponent* OwnerASC = ActorInfo->AbilitySystemComponent.Get();

	if (!OwnerASC)
	{
		return false;
	}

	const UKHAttributeSet_Character* pAttributes  = OwnerASC->GetSet<UKHAttributeSet_Character>();
	if (!pAttributes)
	{
		return false;
	}
	
	if (pAttributes->GetCurrentAmmo() >= pAttributes->GetMaxAmmo())
	{
		return false;
	}

	UE_LOG(LogTemp,Warning,TEXT("CanActiveReload"))
	return true;
}

void UKHGameplayAbility_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 3.0f);
	if (DelayTask)
	{
		UE_LOG(LogTemp,Warning,TEXT("Reload Activate"))
		
		DelayTask->OnFinish.AddDynamic(this, &UKHGameplayAbility_Reload::OnReloadComplete);
		DelayTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UKHGameplayAbility_Reload::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UKHGameplayAbility_Reload::OnReloadComplete()
{
	UE_LOG(LogTemp,Warning,TEXT("Complete Reload"))
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	const UKHAttributeSet_Character* Attributes = ASC->GetSet<UKHAttributeSet_Character>();

	if (ASC && Attributes)
	{
		const float MaxAmmo = Attributes->GetMaxAmmo();
		
		ASC->ApplyModToAttribute(Attributes->GetCurrentAmmoAttribute(), EGameplayModOp::Override, MaxAmmo);
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
