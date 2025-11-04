// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "GameplayTagContainer.h"
#include "KHBTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHBTTask_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
public:
	UKHBTTask_Attack();
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	FGameplayTag AttackAbilityTag;
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
