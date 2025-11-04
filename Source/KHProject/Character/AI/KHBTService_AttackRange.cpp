// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/KHBTService_AttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UKHBTService_AttackRange::UKHBTService_AttackRange()
{
	NodeName = "Attack Range Service";
}

void UKHBTService_AttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (Blackboard == nullptr || AIController == nullptr)
	{
		return;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr)
	{
		return;
	}
	
	AActor* TargetPlayer = Cast<AActor>(Blackboard->GetValueAsObject("TargetPlayer"));

	if (TargetPlayer)
	{
		const float Distance = FVector::Distance(AIPawn->GetActorLocation(), TargetPlayer->GetActorLocation());
		const bool bInRange = (Distance <= AttackRange);

		// 4. "쓸" 키 (bIsInAttackRange)에 계산된 boolean 값을 씁니다.
		Blackboard->SetValueAsBool("InAttackRange", bInRange);
	}
	else
	{
		// 5. 타겟이 없으면 (예: 놓쳤으면) 'bIsInAttackRange'를 false로 씁니다.
		Blackboard->SetValueAsBool("InAttackRange", false);
	}
}
