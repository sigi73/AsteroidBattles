// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "ICursor.h"
#include "ShipController.h"



AShipController::AShipController()
{
	bShowMouseCursor = false;
	bEnableMouseOverEvents = true;

	DefaultMouseCursor = EMouseCursor::Crosshairs;

	bIsControllingShip = true;

	MouseDeltaMultiplier = 1.0f;
	MouseInputDelta = 0.0f;
}

void AShipController::BeginPlay()
{
	if (Cast<ABaseSpaceship>(GetPawn()))
	{
		ControlledShip = Cast<ABaseSpaceship>(GetPawn());
	}
	else
	{
		ConsoleCommand("quit");
	}
}

void AShipController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Thrust", this, &AShipController::AddThrust);
	InputComponent->BindAxis("Roll", this, &AShipController::AddRoll);
}

void AShipController::AddThrust(float Magnitude)
{
	if (bIsControllingShip)
	{
		ControlledShip->AddThrust(Magnitude);
	}
}


void AShipController::AddPitch(float Magnitude)
{
	if (bIsControllingShip)
	{
		ControlledShip->AddPitch(Magnitude);
	}
}

void AShipController::AddRoll(float Magnitude)
{
	if (bIsControllingShip)
	{
		ControlledShip->AddRoll(Magnitude);
	}
}

void AShipController::AddYaw(float Magnitude)
{
	if (bIsControllingShip)
	{
		ControlledShip->AddYaw(Magnitude);
	}
}


void AShipController::Tick(float Magnitude)
{
	if (bIsControllingShip)
	{
		float DeltaX;
		float DeltaY;

		GetInputMouseDelta(DeltaX, DeltaY);


		if (FMath::Abs(DeltaX) > MouseInputDelta)
		{
			AddYaw(DeltaX * MouseDeltaMultiplier);
		}

		if (FMath::Abs(DeltaY) > MouseInputDelta)
		{
			AddPitch(DeltaY * MouseDeltaMultiplier);
		}
	}
}

