// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KHGameplayAbility_AIMelee.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHGameplayAbility_AIMelee : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UKHGameplayAbility_AIMelee();
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FGameplayTag HitCheckEventTag;
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


private:
	UFUNCTION()
	void OnHitCheckEvent(FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageEnded();
};
