// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "KHCharacterBase.h"
#include "KHCharacter_Player.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UGameplayAbility;
UCLASS()
class KHPROJECT_API AKHCharacter_Player : public AKHCharacterBase
{
	GENERATED_BODY()
public:
	AKHCharacter_Player();
public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> FireAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> DefaultCameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> DefaultSpringArmComponent;
protected:
	void Input_Ability_Pressed(EAbilityInputID InputID);
	void Input_Ability_Released(EAbilityInputID InputID);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	
	// Enhanced Input Action 변수 (프로젝트의 IA 에셋을 사용하세요)
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Look;

public:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayImpactFX(const FVector_NetQuantize& HitLocation, const FVector_NetQuantizeNormal& HitNormal);
};
