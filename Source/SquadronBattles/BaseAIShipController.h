// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BaseAIShipController.generated.h"

/**
 * 
 */
UCLASS()
class SQUADRONBATTLES_API ABaseAIShipController : public AAIController
{
	GENERATED_BODY()
	
public:

	ABaseAIShipController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Squadron")
	float ThrustMultiplier;

	FVector TargetLocation;
	FRotator TargetRotation;


private:

	class ABaseSpaceship* ControlledShip;
};
