// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/KHActor_Spawner.h"

#include "KHCharacter_MonsterBase.h"


// Sets default values
AKHActor_Spawner::AKHActor_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKHActor_Spawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKHActor_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKHActor_Spawner::SpawnEnemy(TSubclassOf<AKHCharacter_MonsterBase> _monster)
{
	if (_monster)
	{
		if (AKHCharacter_MonsterBase* pMonster = GetWorld()->SpawnActor<AKHCharacter_MonsterBase>(_monster, GetActorLocation(), GetActorRotation()))
		{
			
		}
	}
}

