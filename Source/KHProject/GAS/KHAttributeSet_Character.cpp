// Fill out your copyright notice in the Description page of Project Settings.


#include "KHAttributeSet_Character.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "KHCharacterBase.h"
#include "KHCharacter_Player.h"
#include "Controller/KHPlayerController_Player.h"

UKHAttributeSet_Character::UKHAttributeSet_Character()
{
	
	InitHealth(0.f);
	InitMaxHealth(100.0f);
	InitMoveSpeed(600.0f);

	InitCurrentAmmo(0.f);
	InitMaxAmmo(30.f);
	UObject* Outer = GetOuter();
	FString OuterName = Outer ? Outer->GetName() : TEXT("NULL");
	InitXP(0.0f);          
	InitMaxXP(3.0f);     
	InitCharacterLevel(1.0f);
	InitDamageMultiValue(1.0f);     
	InitFireCoolMultiValue(1.0f);


	static ConstructorHelpers::FClassFinder<UGameplayEffect> BP_UpdateXP(TEXT("/Game/GAS/GE_UpdateXP.GE_UpdateXP_C"));

	if (BP_UpdateXP.Succeeded())
	{
		m_pUpdateXP = BP_UpdateXP.Class;
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

	if (Attribute == GetXPAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("XP Update Attempt: NewValue=%f, CurrentMaxXP=%f"), NewValue, GetMaxXP());
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxXP());
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
	PostXPExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (AKHCharacterBase* pCharacter = Cast<AKHCharacterBase>(Data.Target.AbilityActorInfo->AvatarActor.Get()))
		{
			if (Data.EvaluatedData.Magnitude <= 0.0f)
			{
				pCharacter->OnHit(Data);
			}
		
			SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

			if (GetHealth() <= 0.0f)
			{
				pCharacter->HealthEmpty(Data);
			}
		}
	}
}

void UKHAttributeSet_Character::PostXPExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetXPAttribute())
	{
		float CurrentXP = GetXP();
		float CurrentMaxXP = GetMaxXP();
		
		if (CurrentXP >= CurrentMaxXP)
		{
			CurrentXP -= CurrentMaxXP; 
			
			float NewLevel = GetCharacterLevel() + 1.0f;
			SetCharacterLevel(NewLevel);
			if (AKHCharacter_Player* pPlayer = Cast<AKHCharacter_Player>(GetOwningActor()))
			{
				if (AKHPlayerController_Player* pPS = Cast<AKHPlayerController_Player>(pPlayer->GetController()))
				{
					pPS->OnLevelUpShowUI();
				}
			}
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
				Context.AddSourceObject(GetOwningActor());
				if (m_pUpdateXP)
				{
					ASC->ApplyGameplayEffectToSelf(m_pUpdateXP.GetDefaultObject(), NewLevel,Context);
				}
			}
		}

		SetXP(CurrentXP); 
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
	DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, XP, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, MaxXP, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, DamageMultiValue, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UKHAttributeSet_Character, FireCoolMultiValue, COND_None, REPNOTIFY_Always);
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

void UKHAttributeSet_Character::OnRep_XP(const FGameplayAttributeData& OldXp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, XP, OldXp);
}

void UKHAttributeSet_Character::OnRep_MaxXP(const FGameplayAttributeData& OldMaxXp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, MaxXP, OldMaxXp);
}

void UKHAttributeSet_Character::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, CharacterLevel, OldCharacterLevel);
}

void UKHAttributeSet_Character::OnRep_DamageMultiValue(const FGameplayAttributeData& OldDamageMultiValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, DamageMultiValue, OldDamageMultiValue);
}

void UKHAttributeSet_Character::OnRep_FireCoolMultiValue(const FGameplayAttributeData& OldFireCoolMultiValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKHAttributeSet_Character, FireCoolMultiValue, OldFireCoolMultiValue);
}
