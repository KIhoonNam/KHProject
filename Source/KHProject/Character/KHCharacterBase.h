// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "KHCharacterBase.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None        UMETA(DisplayName = "None"),
	Confirm     UMETA(DisplayName = "Confirm"),
	Cancel      UMETA(DisplayName = "Cancel"),
	Fire        UMETA(DisplayName = "Fire"),
	Revive    UMETA(DisplayName = "Revive"),
	Reload    UMETA(DisplayName = "Reload")
};

class UAbilitySystemComponent;
class UKHAttributeSet_Character;

UCLASS()
class KHPROJECT_API AKHCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKHCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UKHAttributeSet_Character> m_pAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> BaseStatsEffect;


	
protected:


};
