// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KHGameplayAbility_AIRangeAttack.generated.h"

class AKHActor_Projectlie;
/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHGameplayAbility_AIRangeAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UKHGameplayAbility_AIRangeAttack();
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FGameplayTag HitCheckEventTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AKHActor_Projectlie> ProjectileClass;
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


private:
	UFUNCTION()
	void OnHitCheckEvent(FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageEnded();
	
};
