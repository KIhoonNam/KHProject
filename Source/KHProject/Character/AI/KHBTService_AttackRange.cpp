// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/KHBTService_AttackRange.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "KHCharacter_Player.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


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
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetPlayer);
		if (TargetASC && TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Downed"))))
		{
			TargetPlayer = FindNewActor(TargetPlayer, AIController);

			Blackboard->SetValueAsObject("TargetPlayer", TargetPlayer);
		}
	}

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

AActor* UKHBTService_AttackRange::FindNewActor(AActor* _targetActor, AAIController* _ownerController)
{
	if (_targetActor == nullptr) return nullptr;

	if (_ownerController == nullptr) return nullptr;

	UAIPerceptionComponent* PerceptionComponent = _ownerController->GetPerceptionComponent();
	if (PerceptionComponent == nullptr) return nullptr;

	TArray<AActor*> TargetActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), TargetActors);

	for (AActor* TargetActor : TargetActors)
	{
		// 2. C++ 플레이어 캐릭터인지 C++로 확인
		AKHCharacter_Player* SensedPlayer = Cast<AKHCharacter_Player>(TargetActor);
		if (SensedPlayer == nullptr)
		{
			continue; // C++ 플레이어가 아니면 통과
		}

		// 3. C++ ASC를 C++로 가져와 "빈사" 상태가 "아닌지" C++로 확인
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SensedPlayer);
		if (TargetASC && !TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Downed"))))
		{
			// 4. "살아있는" C++ 플레이어를 찾았습니다! C++로 반환합니다.
			return SensedPlayer;
		}
	}

	return nullptr;
}


