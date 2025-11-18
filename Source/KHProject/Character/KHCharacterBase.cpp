// Fill out your copyright notice in the Description page of Project Settings.


#include "KHCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "../GAS/KHAttributeSet_Character.h" 
#include "GameFramework/PlayerState.h"

// Sets default values
AKHCharacterBase::AKHCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	
	AbilitySystemComponent->SetIsReplicated(true);


	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	
	m_pAttributeSet = CreateDefaultSubobject<UKHAttributeSet_Character>(TEXT("CharacterAttributeSet"));
}

UAbilitySystemComponent* AKHCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AKHCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKHCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKHCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}



void AKHCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!AbilitySystemComponent)
	{
		return;
	}
	
	if (!HasAuthority()) return;




	

}

void AKHCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	// if (APlayerState* PS = GetPlayerState())
	// {
	// 	AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	// }
}




