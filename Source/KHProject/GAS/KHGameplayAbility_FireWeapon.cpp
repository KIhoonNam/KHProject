// Fill out your copyright notice in the Description page of Project Settings.


#include "KHGameplayAbility_FireWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "KHAttributeSet_Character.h"
#include "GameplayAbilitySet.h"
#include "KHProject/Character/KHCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"


UKHGameplayAbility_FireWeapon::UKHGameplayAbility_FireWeapon()
{
	
}

bool UKHGameplayAbility_FireWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                       const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	// 1. 부모 클래스(UGameplayAbility)의 검사를 먼저 통과해야 합니다. (필수)
	// (이 검사가 Cost와 Cooldown을 자동으로 확인해 줍니다)
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// 2. [탄약 검사] AttributeSet을 직접 가져와서 검사합니다.
	const UKHAttributeSet_Character* Attributes = Cast<UKHAttributeSet_Character>(ActorInfo->AbilitySystemComponent->GetSet<UKHAttributeSet_Character>());
	if (Attributes)
	{
		if (Attributes->GetCurrentAmmo() <= 0.0f)
		{
			UE_LOG(LogTemp,Warning,TEXT("Not Ammo"))
			// (선택 사항) 여기에 "총알 없음" 사운드 재생 등을 넣을 수 있습니다.
			return false;
		}
	}
	else
	{
		return false; // 어트리뷰트 셋이 없으면 발사 불가
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
	UE_LOG(LogTemp, Log, TEXT("Active Ability"));
	if (HasAuthority(&ActivationInfo))
	{
		FHitResult HitResult;
		// ... C++ 라인 트레이스 로직 수행 ...

		UKismetSystemLibrary::LineTraceSingle(GetWorld(), 
			ActorInfo->AvatarActor->GetActorLocation(),
			ActorInfo->AvatarActor->GetActorLocation() + ActorInfo->AvatarActor->GetActorForwardVector() * 1000.0f,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
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

			
			FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);

			ApplyGameplayEffectSpecToTarget(Handle, ActorInfo, ActivationInfo, SpecHandle, TargetDataHandle);
			
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
	else
	{

		// UGameplayStatics::SpawnEmitterAttached(...)
		// UGameplayStatics::PlaySoundAtLocation(...)
	}
	
}


