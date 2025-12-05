// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/KHBTTask_Attack.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


UKHBTTask_Attack::UKHBTTask_Attack()
{
	NodeName = "Attack (GAS)";

	// 1-2. [핵심] 이 태스크가 발동시킬 어빌리티 태그를 C++에서 설정합니다.
	AttackAbilityTag = FGameplayTag::RequestGameplayTag(FName("AI.Attack.Melee"));
}

EBTNodeResult::Type UKHBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* pTarget = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetPlayer"));
	if (pTarget == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FRotator FindLookAt = (UKismetMathLibrary::FindLookAtRotation(AIController->GetPawn()->GetActorLocation(), pTarget->GetActorLocation()));
	
	ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	IAbilitySystemInterface* AS_Interface = Cast<IAbilitySystemInterface>(AIPawn);
	if (AS_Interface == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	UAbilitySystemComponent* ASC = AS_Interface->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (AIController->GetFocusActor() == nullptr)
	{
		AIController->SetFocus(pTarget);

		AIPawn->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		AIPawn->GetCharacterMovement()->bOrientRotationToMovement = false;
		AIPawn->GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
		
	}

	float fDistnace = AIPawn->GetActorRotation().Yaw -FindLookAt.Yaw;
	fDistnace = FMath::Abs(fDistnace);
	UE_LOG(LogTemp,Warning,TEXT("Distance : %f"),fDistnace)
	if (fDistnace <= 5.0f)
	{
		bool bSuccess = ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AttackAbilityTag));

		if (bSuccess)
		{
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
			AIPawn->GetCharacterMovement()->bUseControllerDesiredRotation = false;
			AIPawn->GetCharacterMovement()->bOrientRotationToMovement = true;
			AIPawn->GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
			//	AIController->GetPawn()->SetActorRotation(FindLookAt);
			UE_LOG(LogTemp,Warning,TEXT("Attack"))
			// 성공적으로 어빌리티를 발동시켰다면, 비헤이비어 트리에 '성공'을 반환합니다.
			return EBTNodeResult::Succeeded;
		}
	}
	
	


	// (예: 쿨다운 중이거나, 어빌리티가 없어서) 발동에 실패했다면 '실패'를 반환합니다.
	return EBTNodeResult::Failed;
}
