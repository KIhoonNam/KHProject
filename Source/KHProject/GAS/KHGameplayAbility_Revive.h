// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KHGameplayAbility_Revive.generated.h"

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHGameplayAbility_Revive : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UKHGameplayAbility_Revive();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UFUNCTION()
	void OnReviveComplete();

	void HandleOwnerDowned(FGameplayTag GameplayTag, int count);

protected:
	
	UPROPERTY(EditAnywhere)
	float m_fReviveDuration = 5.0f;

	UPROPERTY(EditAnywhere)
	float m_fReviveMaxDistance = 50.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> ReviveEffectClass;

	mutable TWeakObjectPtr<UAbilitySystemComponent> TargetASC_ToRevive;

	FDelegateHandle DownedTagHandle;
};
