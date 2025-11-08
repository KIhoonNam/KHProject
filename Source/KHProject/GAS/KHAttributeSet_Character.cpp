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

	static ConstructorHelpers::FClassFinder<UGameplayEffect> DownedGEFinder(
		TEXT("/Game/GAS/GE_Down.GE_Down_C")
	);
	if (DownedGEFinder.Succeeded())
	{
		DownedEffectClass = DownedGEFinder.Class;
	}
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


	UAbilitySystemComponent* TargetASC = &Data.Target;
	if (!TargetASC)
	{
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
	
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (GetHealth() <= 0.0f)
		{
	
			const FGameplayTag DownedTag = FGameplayTag::RequestGameplayTag(FName("Status.Downed"));
			
			if (!TargetASC->HasMatchingGameplayTag(DownedTag))
			{
				if (DownedEffectClass)
				{
					FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
					Context.AddInstigator(TargetASC->GetAvatarActor(), TargetASC->GetAvatarActor());
					Context.AddSourceObject(TargetASC->GetAvatarActor());
					
					FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(
						DownedEffectClass,
						1.0f, 
						Context
					);
					
					if (SpecHandle.IsValid())
					{
						TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
			
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
