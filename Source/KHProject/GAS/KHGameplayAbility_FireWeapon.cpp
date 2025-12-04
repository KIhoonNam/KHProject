// Fill out your copyright notice in the Description page of Project Settings.


#include "KHGameplayAbility_FireWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilityTask_WaitDelay.h"
#include "KHAttributeSet_Character.h"
#include "GameplayAbilitySet.h"
#include "KHCharacter_Player.h"
#include "AbilityTask_WaitInputRelease.h"
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


	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);

	if (WaitInputReleaseTask)
	{
		WaitInputReleaseTask->OnRelease.AddDynamic(this, &UKHGameplayAbility_FireWeapon::InputReleasedTask);
		WaitInputReleaseTask->ReadyForActivation();
	}
	

	if (HasAuthority(&ActivationInfo))
	{
		IsInput = true;
		Fire();
	}
	// else
	// {
	//
	// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	// }
	
}



void UKHGameplayAbility_FireWeapon::OnFireCool()
{
	if (m_pWeaponData)
	{
		bool isAuto = m_pWeaponData->m_IsAuto;
		if (isAuto && IsInput)
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
	const UKHAttributeSet_Character* Attributes =	GetCurrentActorInfo()->AbilitySystemComponent->GetSet<UKHAttributeSet_Character>();
	if (!Attributes)
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
				int32 Count = m_pWeaponData->m_Bullet;
				TMap<AActor*,float> mapDamage;
				TMap<AActor*, FHitResult> mapResult;
				for (int i = 0 ; i < Count ; i++)
				{
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
						float fDamage = m_pWeaponData->m_fDamage*Attributes->GetDamageMultiValue()/Count;
						if (mapDamage.Contains(HitResult.GetActor()))
						{
							mapDamage[HitResult.GetActor()] += fDamage;
						}
						else
						{
							mapDamage.Emplace(HitResult.GetActor(), fDamage);
							mapResult.Emplace(HitResult.GetActor(), HitResult);
						}

					}
				}

				for (auto& HitMap : mapDamage)
				{
					FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);
			
					if (SpecHandle.IsValid())
					{
						SpecHandle.Data->SetSetByCallerMagnitude(
						FGameplayTag::RequestGameplayTag(FName("Data.Damage")), 
						-HitMap.Value
						);
						
						FGameplayEffectContextHandle ContextHandle = SpecHandle.Data->GetContext().Duplicate();
				
						ContextHandle.AddInstigator(GetCurrentActorInfo()->AvatarActor.Get(), GetCurrentActorInfo()->AvatarActor.Get());
						ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

						SpecHandle.Data->SetContext(ContextHandle);
					}
			
			
					FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(mapResult[HitMap.Key]);

			
					ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), SpecHandle, TargetDataHandle);
				}
				FGameplayCueParameters CueParams;
				CueParams.Location = WeaponMuzzleLocation;
				GetCurrentActorInfo()->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Combat.FireWeapon"),CueParams);
				float fTotalFireCoolDown = m_pWeaponData->m_fCoolDown * Attributes->GetFireCoolMultiValue();
				UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this,fTotalFireCoolDown);
				if (DelayTask)
				{ 
					DelayTask->OnFinish.AddDynamic(this, &UKHGameplayAbility_FireWeapon::OnFireCool);
					DelayTask->ReadyForActivation();
				}
				else
				{
					EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
				}
			}
		}
}

void UKHGameplayAbility_FireWeapon::InputReleasedTask(float TimeHeld)
{
	UE_LOG(LogTemp,Warning,TEXT("Release"))
	IsInput = false;

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}



