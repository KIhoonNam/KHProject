// Fill out your copyright notice in the Description page of Project Settings.


#include "KHActor_Projectlie.h"

#include "AbilitySystemComponent.h"
#include "KHCharacter_Player.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AKHActor_Projectlie::AKHActor_Projectlie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ProjectlieMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectlieMeshComponent"));
	RootComponent = ProjectlieMeshComponent;
	ProjectlieMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectlieMovementComponent"));
	ProjectlieMovementComponent->bRotationFollowsVelocity = true;
	
}

// Called when the game starts or when spawned
void AKHActor_Projectlie::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectlieMeshComponent)
	{
		ProjectlieMeshComponent->OnComponentBeginOverlap.AddDynamic(this,&AKHActor_Projectlie::OnBeginOverlap);
	}
}

// Called every frame
void AKHActor_Projectlie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKHActor_Projectlie::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AKHCharacter_Player* pPlayer = Cast<AKHCharacter_Player>(OtherActor))
	{
		if (UAbilitySystemComponent* pASC = pPlayer->GetAbilitySystemComponent())
		{
			if (DamageEffectClass)
			{
				FGameplayEffectContextHandle EffectContext = pASC->MakeEffectContext();
				EffectContext.AddSourceObject(this);
		
				FGameplayEffectSpecHandle SpecHandle = pASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
			
				if (SpecHandle.IsValid())
				{
					pASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}

