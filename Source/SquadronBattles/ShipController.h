// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "BaseShip.h"
#include "ShipController.generated.h"

/**
 * 
 */
UCLASS()
class SQUADRONBATTLES_API AShipController : public APlayerController
{
	GENERATED_BODY()

public:

	AShipController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShipInput")
	float MouseDeltaMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ShipInput")
	float MouseInputDelta;

private:
	bool bIsControllingShip;

	ABaseShip* ControlledShip;

	void AddThrust(float Magnitude);

	void AddPitch(float Magnitude);
	void AddRoll(float Magnitude);
	void AddYaw(float Magnitude);

	void TakeControlOfShip();

	void FireWeapon();
};
