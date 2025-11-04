// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "KHGameplayAbility_FireWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHGameplayAbility_FireWeapon : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UKHGameplayAbility_FireWeapon();
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FGameplayTag CooldownTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UParticleSystem* ImpactParticle;


	
};
