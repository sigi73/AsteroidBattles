// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class ASTEROIDBATTLES_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile(const FObjectInitializer &ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void InitVelocity(FVector ShootDirection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Firing)
	float Cooldown; //Seconds

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement;
};