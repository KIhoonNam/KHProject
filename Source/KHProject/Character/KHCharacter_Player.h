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
class UKHWidgetComponent;
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
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnRep_PlayerState() override;
	virtual void HealthEmpty() override;
	void OnASCInitialized();

	FDelegateHandle DownedTagEventHandle;
	FDelegateHandle ChannelingTagEventHandle;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> FireAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> ReviveAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayAbility> ReloadAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> DefaultCameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> DefaultSpringArmComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UKHWidgetComponent> PlayerWidgetComponent;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> DownedEffectClass;
	
	bool bASCInitialized = false;

protected:
	void InitPlayerWidget();
	
	void Input_Ability_Pressed(EAbilityInputID InputID);
	void Input_Ability_Released(EAbilityInputID InputID);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void OnDownedTagChanged(FGameplayTag GameplayTag, int count);

	void OnChannelingTagChanged(FGameplayTag GameplayTag, int count);
	
	virtual void HandleDownedState();

	virtual void HandleRecoveredState();
	// Enhanced Input Action 변수 (프로젝트의 IA 에셋을 사용하세요)
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Revive;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Reload;
	
	bool bIsLocallyTryingToCancel;
	
public:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayImpactFX(const FVector_NetQuantize& HitLocation, const FVector_NetQuantizeNormal& HitNormal);

	UFUNCTION(Server, Reliable,WithValidation)
	void Server_CancelAbilityWithTag(FGameplayTag GameplayTag);

public:
	UPROPERTY(Replicated,  ReplicatedUsing = OnRep_IsDowned,BlueprintReadOnly)
	bool m_IsCurrentDowned;

	UFUNCTION()
	void OnRep_IsDowned();
	
};
