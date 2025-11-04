// Fill out your copyright notice in the Description page of Project Settings.


#include "KHAttributeSet_Character.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
UKHAttributeSet_Character::UKHAttributeSet_Character()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMoveSpeed(600.0f);

	InitCurrentAmmo(30.0f);
	InitMaxAmmo(30.0f);
}

void UKHAttributeSet_Character::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	
	if (Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}

	if (Attribute == GetCurrentAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxAmmo());
	}
}

void UKHAttributeSet_Character::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		
		if (GetHealth() <= 0.0f)
		{
			// --- 사망(Death) 또는 빈사(Downed) 처리 로직 ---
			
            
			// (GAS 방식) 또는, 더 좋은 방법은 "Status.Downed" GameplayTag를 owning actor에게 적용하는 것입니다.
			// FGameplayTagContainer TagContainer;
			// TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Status.Downed")));
			// Data.Target.ApplyGameplayEffectToSelf(/* 이 태그를 적용하는 GameplayEffect */, 1.0f, Data.Target.MakeEffectContext());

			// TODO: [기훈님] 여기에 사망/빈사 처리 로직을 연결하세요.
		}
	}
}

void UKHAttributeSet_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, CurrentAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, MaxAmmo, COND_None, REPNOTIFY_Always);
}

void UKHAttributeSet_Character::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, Health, OldHealth);
}

void UKHAttributeSet_Character::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, MaxHealth, OldMaxHealth);
}

void UKHAttributeSet_Character::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, MoveSpeed, OldMoveSpeed);
}

void UKHAttributeSet_Character::OnRep_CurrentAmmo(const FGameplayAttributeData& OldCurrentAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, CurrentAmmo, OldCurrentAmmo);
}

void UKHAttributeSet_Character::OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, MaxAmmo, OldMaxAmmo);
}
