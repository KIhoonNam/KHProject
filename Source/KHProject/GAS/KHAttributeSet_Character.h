// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "KHAttributeSet_Character.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class KHPROJECT_API UKHAttributeSet_Character : public UAttributeSet
{
	GENERATED_BODY()
public:

	UKHAttributeSet_Character();

	TSubclassOf<UGameplayEffect> m_pUpdateXP;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UKHAttributeSet_Character, Health)


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UKHAttributeSet_Character, MaxHealth)


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UKHAttributeSet_Character, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAmmo)
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS(UKHAttributeSet_Character, MaxAmmo)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentAmmo)
	FGameplayAttributeData CurrentAmmo;
	ATTRIBUTE_ACCESSORS(UKHAttributeSet_Character, CurrentAmmo)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_XP)
    FGameplayAttributeData XP;
    ATTRIBUTE_ACCESSORS(UKHAttributeSet_Character, XP)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxXP)
    FGameplayAttributeData MaxXP;
    ATTRIBUTE_ACCESSORS(UKHAttributeSet_Character, MaxXP)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CharacterLevel)
    FGameplayAttributeData CharacterLevel;
    ATTRIBUTE_ACCESSORS(UKHAttributeSet_Character, CharacterLevel)


	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void PostXPExecute(const FGameplayEffectModCallbackData& Data);
	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
	
	UFUNCTION()
	virtual void OnRep_CurrentAmmo(const FGameplayAttributeData& OldCurrentAmmo);

	UFUNCTION()
	virtual void OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo);
	
	UFUNCTION()
    virtual void OnRep_XP(const FGameplayAttributeData& OldXP);
    
    UFUNCTION()
    virtual void OnRep_MaxXP(const FGameplayAttributeData& OldMaxXP);
   
    UFUNCTION()
    virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);
};
