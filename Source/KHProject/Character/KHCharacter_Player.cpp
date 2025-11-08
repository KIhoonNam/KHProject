// Fill out your copyright notice in the Description page of Project Settings.


#include "KHCharacter_Player.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KHAttributeSet_Character.h"
#include "UnrealNetwork.h"
#include "Anim/KHAnimInstance_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"


AKHCharacter_Player::AKHCharacter_Player()
{
	DefaultCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("DefaultCameraComponent"));


	DefaultSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("DefaultSpringArmComponent"));
	DefaultSpringArmComponent->SetupAttachment(GetRootComponent());
	DefaultSpringArmComponent->TargetArmLength = 300.0f;
	DefaultSpringArmComponent->bUsePawnControlRotation = true;

	DefaultCameraComponent->SetupAttachment(DefaultSpringArmComponent);

	bASCInitialized = false;

	DownedTagEventHandle.Reset();
	ChannelingTagEventHandle.Reset();

	
	
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
		EnhancedInput->BindAction(IA_Revive, ETriggerEvent::Started, this, &AKHCharacter_Player::Input_Ability_Pressed, EAbilityInputID::Revive);
	}
}

void AKHCharacter_Player::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh())
	{
		// 서버(및 모든 곳)에서 항상 애니메이션 포즈를 틱하고 본을 갱신하도록 강제합니다.
		GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
		GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	}
}

void AKHCharacter_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;
	if (bASCInitialized) return;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PossessedBy (SERVER): AbilitySystemComponent is NULL!"));
		return;
	}
	
	APlayerState* PS = GetPlayerState();
	if (PS)
	{
		bASCInitialized = true;
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		OnASCInitialized();
		
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PossessedBy (SERVER): GetPlayerState() is NULL! ASC Init Failed."));
	}



}

void AKHCharacter_Player::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKHCharacter_Player, m_IsDowned);
}

void AKHCharacter_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	if (IsLocallyControlled())
	{
		const FGameplayTag ChannelingTag = FGameplayTag::RequestGameplayTag(FName("Ability.IsChanneling.Revive"));
        
		if (AbilitySystemComponent->HasMatchingGameplayTag(ChannelingTag))
		{
			const float CurrentSpeed = GetCharacterMovement()->Velocity.Size();
			
			if (CurrentSpeed > 0.0f && !bIsLocallyTryingToCancel)
			{

				bIsLocallyTryingToCancel = true;
				Server_CancelAbilityWithTag(ChannelingTag);
				
		
			}
		}
	}
}

void AKHCharacter_Player::OnRep_PlayerState()
{
	
	Super::OnRep_PlayerState();
	if (HasAuthority()) return; 
	if (bASCInitialized) return; 

	if (!AbilitySystemComponent)
	{
		return;
	}

	APlayerState* PS = GetPlayerState();
	if (PS)
	{
	
	
        
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);


		OnASCInitialized(); 
		
		const FGameplayTag DownedTag = FGameplayTag::RequestGameplayTag(FName("Status.Downed"));
		if (AbilitySystemComponent->HasMatchingGameplayTag(DownedTag))
		{
			OnDownedTagChanged(DownedTag, 1);
		}

		bASCInitialized = true;
	}
	else
	{
	}
}

void AKHCharacter_Player::OnASCInitialized()
{
	

	if (AbilitySystemComponent && !DownedTagEventHandle.IsValid())
	{
		DownedTagEventHandle = AbilitySystemComponent->RegisterGameplayTagEvent(
		   FGameplayTag::RequestGameplayTag(FName("Status.Downed")),
		   EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &AKHCharacter_Player::OnDownedTagChanged);
	}
	
	if (AbilitySystemComponent && !ChannelingTagEventHandle.IsValid() )
	{
		ChannelingTagEventHandle = AbilitySystemComponent->RegisterGameplayTagEvent(
		   FGameplayTag::RequestGameplayTag(FName("Ability.IsChanneling.Revive")),
		   EGameplayTagEventType::NewOrRemoved
		).AddUObject(this, &AKHCharacter_Player::OnChannelingTagChanged);
	}
	
	if (HasAuthority())
	{
		if (FireAbility)
		{
			FGameplayAbilitySpec FireAbilitySpec(FireAbility, 1.0f, static_cast<int32>(EAbilityInputID::Fire), this);
			AbilitySystemComponent->GiveAbility(FireAbilitySpec);
		}
		if (ReviveAbility)
		{
			FGameplayAbilitySpec ReviveAbilitySpec(ReviveAbility, 1.0f, static_cast<int32>(EAbilityInputID::Revive), this); 
			AbilitySystemComponent->GiveAbility(ReviveAbilitySpec);
		}
	}
}

void AKHCharacter_Player::Server_CancelAbilityWithTag_Implementation(FGameplayTag GameplayTag)
{
	FGameplayTagContainer TagsToCancelContainer;
	TagsToCancelContainer.AddTag(GameplayTag);

	UE_LOG(LogTemp,Warning,TEXT("%s"),*GameplayTag.ToString())
	
	AbilitySystemComponent->CancelAbilities(&TagsToCancelContainer, nullptr, nullptr);
}

bool AKHCharacter_Player::Server_CancelAbilityWithTag_Validate(FGameplayTag GameplayTag)
{
	return GameplayTag.IsValid();
}

void AKHCharacter_Player::OnRep_IsDowned()
{
	
}

void AKHCharacter_Player::Input_Ability_Pressed(EAbilityInputID InputID)
{

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(InputID));

		UE_LOG(LogTemp,Warning,TEXT("Revive "))
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

		HandleDownedState();
	}
	else 
	{
		HandleRecoveredState();
	}
}

void AKHCharacter_Player::OnChannelingTagChanged(FGameplayTag GameplayTag, int count)
{


	if (count == 0)
	{
		bIsLocallyTryingToCancel = false;
	}
}

void AKHCharacter_Player::HandleDownedState()
{

	m_IsDowned = true;
	
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
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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

	m_IsDowned = false;
}

void AKHCharacter_Player::Multicast_PlayImpactFX_Implementation(const FVector_NetQuantize& HitLocation,
const FVector_NetQuantizeNormal& HitNormal)
{
}
