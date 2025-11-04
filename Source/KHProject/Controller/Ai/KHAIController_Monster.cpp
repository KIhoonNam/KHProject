// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Ai/KHAIController_Monster.h"

#include "KHCharacter_MonsterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"


AKHAIController_Monster::AKHAIController_Monster()
{

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));


	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));


	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 2000.0f; 
		SightConfig->LoseSightRadius = 2500.0f; 
		SightConfig->PeripheralVisionAngleDegrees = 90.0f; 


		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;


		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}
}

void AKHAIController_Monster::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn == nullptr)
	{
		return;
	}

	AKHCharacter_MonsterBase* AICharacter = Cast<AKHCharacter_MonsterBase>(InPawn);
	if (AICharacter && BehaviorTreeAsset)
	{
		// 2-1. [핵심] 블랙보드를 초기화하고 비헤이비어 트리를 실행합니다.
		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);

		// 2-2. [핵심] AI 감지 컴포넌트의 이벤트 핸들러를 이 C++ 함수(OnTargetPerceived)에 바인딩합니다.
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AKHAIController_Monster::OnTargetPerceived);
	}
}

void AKHAIController_Monster::StopBehaviorTree()
{
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void AKHAIController_Monster::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Actor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			SetTargetPlayer(Actor);
		}
	}
	else 
	{
		if (BlackboardComponent->GetValueAsObject(TEXT("TargetPlayer")) == Actor)
		{
			ClearTargetPlayer();
		}
	}
}

void AKHAIController_Monster::SetTargetPlayer(AActor* Target)
{
	if (BlackboardComponent)
	{
		
		BlackboardComponent->SetValueAsObject(TEXT("TargetPlayer"), Target);
	}
}

void AKHAIController_Monster::ClearTargetPlayer()
{
	if (BlackboardComponent)
	{
		
		BlackboardComponent->ClearValue(TEXT("TargetPlayer"));
	}
}
