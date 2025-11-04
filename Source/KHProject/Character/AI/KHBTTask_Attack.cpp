// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/KHBTTask_Attack.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"


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
	
	APawn* AIPawn = AIController->GetPawn();
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
	
	bool bSuccess = ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(AttackAbilityTag));

	if (bSuccess)
	{

		UE_LOG(LogTemp,Warning,TEXT("Attack"))
		// 성공적으로 어빌리티를 발동시켰다면, 비헤이비어 트리에 '성공'을 반환합니다.
		return EBTNodeResult::Succeeded;
	}

	// (예: 쿨다운 중이거나, 어빌리티가 없어서) 발동에 실패했다면 '실패'를 반환합니다.
	return EBTNodeResult::Failed;
}
