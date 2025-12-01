// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KHGameplayAbility_AIMelee.h"
#include "Character/KHCharacterBase.h"
#include "KHCharacter_MonsterBase.generated.h"

class UKHGameplayAbility_AIRangeAttack;
/**
 * 
 */
UCLASS()
class KHPROJECT_API AKHCharacter_MonsterBase : public AKHCharacterBase
{
	GENERATED_BODY()
	AKHCharacter_MonsterBase();
	
public:
	UFUNCTION(BlueprintCallable, Category = "GAS")
	float GetHealth() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayEffect> m_pGiveExp;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UKHGameplayAbility_AIMelee> m_pAIMeleeAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UKHGameplayAbility_AIRangeAttack> m_pAIRangeAbility;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TObjectPtr<UAnimMontage> DieMontage;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent>	WeaponSkeletalMeshComponent;
public:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayMeleeAttackMontage(UAnimMontage* MontageToPlay);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_MonsterDie(UAnimMontage* MontageToPlay);
public:
	virtual void BeginPlay() override;


	virtual void PossessedBy(AController* NewController) override;


	FVector GetWeaponAttackLocation() const;
public:
	void Die();

	virtual void HealthEmpty(const FGameplayEffectModCallbackData& Data) override;

	virtual void OnHit(const FGameplayEffectModCallbackData& Data) override;
};
