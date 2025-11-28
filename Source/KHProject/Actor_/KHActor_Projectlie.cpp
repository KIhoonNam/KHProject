// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_/KHActor_Projectlie.h"

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

	
}

// Called every frame
void AKHActor_Projectlie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

