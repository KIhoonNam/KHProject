// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KHAIController_Monster.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHAIController_Monster : public AAIController
{
	GENERATED_BODY()
	

public:
	AKHAIController_Monster();
	
	virtual void OnPossess(APawn* InPawn) override;
	
	void StopBehaviorTree();

public:

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    TObjectPtr<UBlackboardComponent> BlackboardComponent;




    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	
    UPROPERTY()
    TObjectPtr<UAISenseConfig_Sight> SightConfig;


    UFUNCTION()
    void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus);

public:
    // [헬퍼 함수] 블랙보드의 '타겟' 키에 액터를 설정하는 함수 (나중에 C++에서 사용)
    void SetTargetPlayer(AActor* Target);

    // [헬퍼 함수] 블랙보드의 '타겟' 키를 비우는 함수
    void ClearTargetPlayer();
};
