// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/KHGameplayAbility_AIMelee.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "KHCharacter_MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"


UKHGameplayAbility_AIMelee::UKHGameplayAbility_AIMelee()
{
	HitCheckEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Attack.HitCheck"));
}

void UKHGameplayAbility_AIMelee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!HasAuthority(&ActivationInfo) || AttackMontage == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// --- 태스크 1: 몽타주 재생 ---
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, AttackMontage, 1.0f, NAME_None, false
	);

	if (MontageTask == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 몽타주가 종료되면 (성공/취소/중단) OnMontageEnded 함수를 호출합니다.
	MontageTask->OnCompleted.AddDynamic(this, &UKHGameplayAbility_AIMelee::OnMontageEnded);
	MontageTask->OnInterrupted.AddDynamic(this, &UKHGameplayAbility_AIMelee::OnMontageEnded);
	MontageTask->OnCancelled.AddDynamic(this, &UKHGameplayAbility_AIMelee::OnMontageEnded);
	// 몽타주 재생을 "준비"시킵니다.
	MontageTask->ReadyForActivation();

	// --- 태스크 2: 이벤트 대기 ---
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,               // 이 어빌리티가 소유
		HitCheckEventTag,   // [핵심] 이 태그가 오는지 대기합니다
		nullptr,            // (Optional) 이벤트의 'Source' (지금은 필요 없음)
		true                // 'Owner'에게서만 이벤트를 수신
	);

	if (EventTask == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// [핵심] 이벤트가 수신되면, OnHitCheckEvent C++ 함수를 호출합니다.
	EventTask->EventReceived.AddDynamic(this, &UKHGameplayAbility_AIMelee::OnHitCheckEvent);
	// 이벤트 대기를 "준비"시킵니다.
	EventTask->ReadyForActivation();
}



void UKHGameplayAbility_AIMelee::OnHitCheckEvent(FGameplayEventData Payload)
{
	UE_LOG(LogTemp,Warning,TEXT("OnHitCheckEvent"));	
	AAIController* AIController = Cast<AAIController>(GetActorInfo().OwnerActor.Get());
	UBlackboardComponent* Blackboard = AIController ? AIController->GetBlackboardComponent() : nullptr;
	AActor* TargetPlayer = Blackboard ? Cast<AActor>(Blackboard->GetValueAsObject(TEXT("TargetPlayer"))) : nullptr;

	if (TargetPlayer)
	{

		FHitResult HitResult;
		
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
			GetActorInfo().AvatarActor->GetActorLocation(),
			TargetPlayer->GetActorLocation(),
			50.0f, // 반지름
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), // 충돌 채널
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::ForDuration,
			HitResult,
			true
			,FLinearColor::Red, FLinearColor::Green,
			5.0f
		);

		if (HitResult.GetActor() == TargetPlayer)
		{
			FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(TargetPlayer);
			FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(
				DamageEffectClass,
				GetAbilityLevel()
			);

			if (DamageSpecHandle.IsValid())
			{
				FGameplayEffectContextHandle ContextHandle = DamageSpecHandle.Data->GetContext().Duplicate();
				
				ContextHandle.AddInstigator(GetCurrentActorInfo()->AvatarActor.Get(), GetCurrentActorInfo()->AvatarActor.Get());
				ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

				DamageSpecHandle.Data->SetContext(ContextHandle);
			}
			
			ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), DamageSpecHandle, TargetDataHandle);
		}
		else
		{
			return; // 빗나감
		}
		

	}
}

void UKHGameplayAbility_AIMelee::OnMontageEnded()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
