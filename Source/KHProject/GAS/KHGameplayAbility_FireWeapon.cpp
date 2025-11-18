// Fill out your copyright notice in the Description page of Project Settings.


#include "KHGameplayAbility_FireWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "KHAttributeSet_Character.h"
#include "GameplayAbilitySet.h"
#include "KHProject/Character/KHCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"


UKHGameplayAbility_FireWeapon::UKHGameplayAbility_FireWeapon()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

bool UKHGameplayAbility_FireWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                       const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	const UKHAttributeSet_Character* Attributes = Cast<UKHAttributeSet_Character>(ActorInfo->AbilitySystemComponent->GetSet<UKHAttributeSet_Character>());
	if (Attributes)
	{
		if (Attributes->GetCurrentAmmo() <= 0.0f)
		{
			UE_LOG(LogTemp,Warning,TEXT("Not Ammo"))
			return false;
		}
	}
	else
	{
		return false;
	}
	

	
	return true;
}

void UKHGameplayAbility_FireWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (HasAuthority(&ActivationInfo))
	{
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), 
			ActorInfo->AvatarActor->GetActorLocation(),
			ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 1000.0f,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::ForDuration,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.0f);
		
		if (HitResult.bBlockingHit && HitResult.GetActor())
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);
			
			if (SpecHandle.IsValid())
			{
				FGameplayEffectContextHandle ContextHandle = SpecHandle.Data->GetContext().Duplicate();
				
				ContextHandle.AddInstigator(GetCurrentActorInfo()->AvatarActor.Get(), GetCurrentActorInfo()->AvatarActor.Get());
				ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

				SpecHandle.Data->SetContext(ContextHandle);
			}
			
			
			FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);

			
			ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo, SpecHandle, TargetDataHandle);
			
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
	else
	{


	}
	
}


