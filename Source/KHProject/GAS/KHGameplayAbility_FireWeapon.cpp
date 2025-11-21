// Fill out your copyright notice in the Description page of Project Settings.


#include "KHGameplayAbility_FireWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilityTask_WaitDelay.h"
#include "KHAttributeSet_Character.h"
#include "GameplayAbilitySet.h"
#include "KHCharacter_Player.h"
#include "DataTable/KHDataTable_PlayerAnim.h"
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


	if (HasAuthority(&ActivationInfo))
	{
		Fire();
	}
	else
	{

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
	
}

void UKHGameplayAbility_FireWeapon::OnFireCool()
{
	if (m_pWeaponData)
	{
		bool isAuto = m_pWeaponData->m_IsAuto;
		if (isAuto && GetCurrentAbilitySpec()->InputPressed)
		{
			Fire();
		}
		else
		{
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
	}
}

void UKHGameplayAbility_FireWeapon::Fire()
{

	if (!CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}
	
	if (AKHCharacter_Player* pPlayer = Cast<AKHCharacter_Player>(GetCurrentActorInfo()->AvatarActor.Get()))
		{
			if (APlayerController* pPS = Cast<APlayerController>(pPlayer->GetController()))
			{
				FString FireMontageName = TEXT("Fire_") + EnumToString(pPlayer->m_eWeaponType);
				if (UAnimMontage* FireMontage = pPlayer->GetAnimMontage(*FireMontageName))
				{
					pPlayer->Multicast_PlayAnimMontage(FireMontage);
				}
				m_pWeaponData = pPlayer->GetWeaponData();
				if (!m_pWeaponData)
				{
					EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
					return;
				}
				FVector vecCamera;
				FRotator rotCamera;
				pPS->GetPlayerViewPoint(vecCamera,rotCamera);
				FVector StartPoint = vecCamera;
				FVector EndPoint = StartPoint + rotCamera.Vector() * 10000.0f;
				FHitResult CameraHitResult;
				UKismetSystemLibrary::LineTraceSingle(GetWorld(), 
					StartPoint,
					EndPoint,
					UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
					false,
					TArray<AActor*>(),
					EDrawDebugTrace::ForDuration,
					CameraHitResult,
					true,
					FLinearColor::Red,
					FLinearColor::Green,
					5.0f);

			
				FVector WeaponMuzzleLocation = pPlayer->GetAimStartLocation();
				FVector CameraEndPoint = WeaponMuzzleLocation + FVector(pPlayer->GetAimEndRotation())*10000.0f;
				if (CameraHitResult.bBlockingHit && CameraHitResult.GetActor())
				{
					CameraEndPoint = CameraHitResult.Location;
				}

				FVector vecDir = (CameraEndPoint - WeaponMuzzleLocation).GetSafeNormal();
				if (m_pWeaponData->SpreadAngle > 0.0f)
				{
					// 라디안으로 변환 후 랜덤 벡터 생성
					float SpreadRad = FMath::DegreesToRadians(m_pWeaponData->SpreadAngle);
					vecDir = FMath::VRandCone(vecDir, SpreadRad);
				}
				FHitResult HitResult;
				UKismetSystemLibrary::LineTraceSingle(GetWorld(), 
					WeaponMuzzleLocation,
					WeaponMuzzleLocation +vecDir *10000.0f,
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
						SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(FName("Data.Damage")), 
			-m_pWeaponData->m_fDamage 
		);
						
						FGameplayEffectContextHandle ContextHandle = SpecHandle.Data->GetContext().Duplicate();
				
						ContextHandle.AddInstigator(GetCurrentActorInfo()->AvatarActor.Get(), GetCurrentActorInfo()->AvatarActor.Get());
						ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

						SpecHandle.Data->SetContext(ContextHandle);
					}
			
			
					FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);

			
					ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), SpecHandle, TargetDataHandle);

					UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, m_pWeaponData->m_fCoolDown);
					if (DelayTask)
					{
						DelayTask->OnFinish.AddDynamic(this, &UKHGameplayAbility_FireWeapon::OnFireCool);
						DelayTask->ReadyForActivation();
					}
				}
			}
		}
}



