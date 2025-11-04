// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KHGameplayAbility_AIMelee.h"
#include "Character/KHCharacterBase.h"
#include "KHCharacter_MonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHCharacter_MonsterBase : public AKHCharacterBase
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category = "GAS")
	float GetHealth() const;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UKHGameplayAbility_AIMelee> m_pAIMeleeAbility;
public:
	virtual void BeginPlay() override;


	virtual void PossessedBy(AController* NewController) override;
};
