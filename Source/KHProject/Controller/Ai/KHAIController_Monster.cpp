// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Ai/KHAIController_Monster.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "KHCharacter_MonsterBase.h"
#include "KHCharacter_Player.h"
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
		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
		
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
	UObject* TargetActorObject = BlackboardComponent->GetValueAsObject(TEXT("TargetPlayer"));
	
	if (Stimulus.WasSuccessfullySensed())
	{
		AKHCharacter_Player* SensedPlayer = Cast<AKHCharacter_Player>(Actor);

		if (SensedPlayer && TargetActorObject == nullptr)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SensedPlayer);
            
			if (TargetASC && !TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Downed"))))
			{
				SetTargetPlayer(SensedPlayer);
			}
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
		if (AActor* pTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(TEXT("TargetPlayer"))))
		{
			BlackboardComponent->SetValueAsVector(TEXT("LastPos"), pTarget->GetActorLocation());
		}
	
		BlackboardComponent->ClearValue(TEXT("TargetPlayer"));
	}
}
