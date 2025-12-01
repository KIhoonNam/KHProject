// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KHActor_Projectlie.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class KHPROJECT_API AKHActor_Projectlie : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKHActor_Projectlie();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ProjectlieMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectlieMovementComponent;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass;
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
