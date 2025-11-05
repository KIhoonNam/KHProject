// Fill out your copyright notice in the Description page of Project Settings.


#include "KHCharacter_Player.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KHAttributeSet_Character.h"
#include "Anim/KHAnimInstance_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AKHCharacter_Player::AKHCharacter_Player()
{
	DefaultCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("DefaultCameraComponent"));


	DefaultSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("DefaultSpringArmComponent"));
	DefaultSpringArmComponent->SetupAttachment(GetRootComponent());
	DefaultSpringArmComponent->TargetArmLength = 300.0f;
	DefaultSpringArmComponent->bUsePawnControlRotation = true;

	DefaultCameraComponent->SetupAttachment(DefaultSpringArmComponent);
}

void AKHCharacter_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		return;
	}
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}
	
	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}
    
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		return;
	}
	
	if (DefaultInputMappingContext)
	{
		Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
	}

	
	if (IA_Fire)
	{
		
		EnhancedInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &AKHCharacter_Player::Input_Ability_Pressed, EAbilityInputID::Fire);
		EnhancedInput->BindAction(IA_Fire, ETriggerEvent::Completed, this, &AKHCharacter_Player::Input_Ability_Released, EAbilityInputID::Fire);
		EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AKHCharacter_Player::Input_Move);
		EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AKHCharacter_Player::Input_Look);
	}
}

void AKHCharacter_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
	if (AbilitySystemComponent && FireAbility)
	{
		FGameplayAbilitySpec FireAbilitySpec(
			FireAbility, 
			1.0f, 
			static_cast<int32>(EAbilityInputID::Fire),
			this // Owner
		);
		
		AbilitySystemComponent->GiveAbility(FireAbilitySpec);

		AbilitySystemComponent->RegisterGameplayTagEvent(
		FGameplayTag::RequestGameplayTag(FName("Status.Downed")),
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(this, &AKHCharacter_Player::OnDownedTagChanged);
	}
	
}

void AKHCharacter_Player::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->RegisterGameplayTagEvent(
		FGameplayTag::RequestGameplayTag(FName("Status.Downed")),
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(this, &AKHCharacter_Player::OnDownedTagChanged);
}

void AKHCharacter_Player::Input_Ability_Pressed(EAbilityInputID InputID)
{

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(InputID));

		UE_LOG(LogTemp,Warning,TEXT("Helath %f"),AbilitySystemComponent->GetNumericAttribute(UKHAttributeSet_Character::GetHealthAttribute()))
	}
}

void AKHCharacter_Player::Input_Ability_Released(EAbilityInputID InputID)
{

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(InputID));
	}
}

void AKHCharacter_Player::Input_Move(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AKHCharacter_Player::Input_Look(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void AKHCharacter_Player::OnDownedTagChanged(FGameplayTag GameplayTag, int count)
{
	if (count > 0)
	{

		//HandleDownedState();
	}
	else 
	{
		//HandleRecoveredState();
	}
}

void AKHCharacter_Player::HandleDownedState()
{
	if (AController* pController = GetController())
	{
		pController->DisableInput(Cast<APlayerController>(pController));
		pController->SetIgnoreLookInput(true);
	}
    

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (UKHAnimInstance_Player* pAnim = Cast<UKHAnimInstance_Player>(GetMesh()->GetAnimInstance()))
	{
		pAnim->SetIsDowned(true);
	}
}

void AKHCharacter_Player::HandleRecoveredState()
{
	if (AController* pController = GetController())
	{
		pController->EnableInput(Cast<APlayerController>(pController));
		pController->SetIgnoreLookInput(false);
	}
	
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
    
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetCharacterMovement()->SetComponentTickEnabled(true);
	}

	if (UKHAnimInstance_Player* pAnim = Cast<UKHAnimInstance_Player>(GetMesh()->GetAnimInstance()))
	{
		pAnim->SetIsDowned(false);
	}
}

void AKHCharacter_Player::Multicast_PlayImpactFX_Implementation(const FVector_NetQuantize& HitLocation,
const FVector_NetQuantizeNormal& HitNormal)
{
}
