// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/KHGameplayAbility_AIMelee.h"
#include "AbilityTask_WaitDelay.h"
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


	EventTask->EventReceived.AddDynamic(this, &UKHGameplayAbility_AIMelee::OnHitCheckEvent);

	EventTask->ReadyForActivation();

	const float MontageDuration = AttackMontage->GetPlayLength();
    
	
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, MontageDuration);
	if (DelayTask)
	{
		// 딜레이가 끝나면 OnMontageEnded (기존에 C++로 만든 함수)를 호출합니다.
		DelayTask->OnFinish.AddDynamic(this, &UKHGameplayAbility_AIMelee::OnMontageEnded);
		DelayTask->ReadyForActivation();
	}
}



void UKHGameplayAbility_AIMelee::OnHitCheckEvent(FGameplayEventData Payload)
{
	UE_LOG(LogTemp,Warning,TEXT("OnHitCheckEvent"));
	AKHCharacter_MonsterBase* AICharacter = Cast<AKHCharacter_MonsterBase>(GetActorInfo().AvatarActor.Get());
	if (!AICharacter) return;
	AAIController* AIController = Cast<AAIController>(AICharacter->GetController());
	UBlackboardComponent* Blackboard = AIController ? AIController->GetBlackboardComponent() : nullptr;
	AActor* TargetPlayer = Blackboard ? Cast<AActor>(Blackboard->GetValueAsObject(TEXT("TargetPlayer"))) : nullptr;

	if (TargetPlayer)
	{

		FHitResult HitResult;
		
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
			GetActorInfo().AvatarActor->GetActorLocation(),
			GetActorInfo().AvatarActor->GetActorLocation() + GetActorInfo().AvatarActor->GetActorForwardVector()*25.f,
			50.0f, 
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
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
