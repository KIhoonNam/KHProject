// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "KHBTService_AttackRange.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHBTService_AttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UKHBTService_AttackRange();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;



public:
	AActor* FindNewActor(AActor* _targetActor,AAIController* _ownerController);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackRange = 150.0f;
};
