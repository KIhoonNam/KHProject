// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/KHGameplayAbility_Revive.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityTask_WaitDelay.h"
#include "KHCharacter_Player.h"
#include "Kismet/KismetSystemLibrary.h"


UKHGameplayAbility_Revive::UKHGameplayAbility_Revive()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Input.Interact")));

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.IsChanneling.Revive")));
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Status.Downed")));
}

bool UKHGameplayAbility_Revive::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                   const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	AActor* Avatar = ActorInfo->AvatarActor.Get();
	if (Avatar == nullptr) return false;


	FVector TraceStart = Avatar->GetActorLocation();
	FVector TraceEnd = TraceStart + (Avatar->GetActorForwardVector() * m_fReviveMaxDistance);

	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Avatar);

	bool bHit = UKismetSystemLibrary::SphereTraceMultiByProfile(
		Avatar->GetWorld(),
		TraceStart,
		TraceEnd,
		m_fReviveMaxDistance, 
		TEXT("Pawn"), 
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHits,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.0f
	);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AKHCharacter_Player* SensedPlayer = Cast<AKHCharacter_Player>(Hit.GetActor());
			if (SensedPlayer)
			{
				UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SensedPlayer);
				if (TargetASC && TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Downed"))))
				{
					TargetASC_ToRevive = TargetASC; 
					return true;
				}
			}
		}
	}
	
	TargetASC_ToRevive.Reset();
	return false;
	
}

void UKHGameplayAbility_Revive::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	if (!HasAuthority(&ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	if (OwnerASC)
	{
		// 2. C++ 자신의 C++ 'Status.Downed' C++ 태그 C++ 이벤트를 C++ 'HandleOwnerDowned' C++ 함수에 C++ 바인딩합니다.
		DownedTagHandle = OwnerASC->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Status.Downed")),
			EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &UKHGameplayAbility_Revive::HandleOwnerDowned);
	}

	
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, m_fReviveDuration);
	if (DelayTask)
	{
		DelayTask->OnFinish.AddDynamic(this, &UKHGameplayAbility_Revive::OnReviveComplete);

		TargetASC_ToRevive = OwnerASC;
		DelayTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}



void UKHGameplayAbility_Revive::OnReviveComplete()
{
	UAbilitySystemComponent* TargetASC = TargetASC_ToRevive.Get();
    
	if (TargetASC)
	{
		const FGameplayTag DownedTag = FGameplayTag::RequestGameplayTag(FName("Status.Downed"));
		FGameplayTagContainer DownedTagContainer;
		DownedTagContainer.AddTag(DownedTag);
		
		TargetASC->RemoveActiveEffectsWithGrantedTags(DownedTagContainer);
		
		if (ReviveEffectClass)
		{
			FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(TargetASC->GetAvatarActor());

            
			FGameplayEffectSpecHandle ReviveSpecHandle = MakeOutgoingGameplayEffectSpec(ReviveEffectClass,GetAbilityLevel());
			if (ReviveSpecHandle.IsValid())
			{
				FGameplayEffectContextHandle ContextHandle = ReviveSpecHandle.Data->GetContext().Duplicate();
				
				ContextHandle.AddInstigator(GetCurrentActorInfo()->AvatarActor.Get(), GetCurrentActorInfo()->AvatarActor.Get());
				ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

				ReviveSpecHandle.Data->SetContext(ContextHandle);
			}
			ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), ReviveSpecHandle, TargetDataHandle);
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UKHGameplayAbility_Revive::HandleOwnerDowned(FGameplayTag GameplayTag, int count)
{
	if (count > 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("Cancel Revive Ability because Owner Downed"))
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}

void UKHGameplayAbility_Revive::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	if (OwnerASC)
	{
		OwnerASC->RegisterGameplayTagEvent(
			FGameplayTag::RequestGameplayTag(FName("Status.Downed")),
			EGameplayTagEventType::NewOrRemoved
		).Remove(DownedTagHandle);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}