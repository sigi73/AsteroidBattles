// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "ICursor.h"
#include "ShipController.h"



AShipController::AShipController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = false;
	bEnableMouseOverEvents = true;

	DefaultMouseCursor = EMouseCursor::Crosshairs;

	bIsControllingShip = false;

	MouseDeltaMultiplier = 1.0f;
	MouseInputDelta = 0.0f;

	ControlledShip = NULL;
}

void AShipController::BeginPlay()
{
	TakeControlOfShip();
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
		UE_LOG(LogTemp, Warning, TEXT("Adding thrust: %f"), Magnitude);
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


void AShipController::Tick(float DeltaSeconds)
{
	if (!bIsControllingShip)
	{
		TakeControlOfShip();
		UE_LOG(LogTemp, Warning, TEXT("Ship found, ID: %s"), *GetName());
	}
	else
	{
		float DeltaX = 0.0f;
		float DeltaY = 0.0f;

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

void AShipController::TakeControlOfShip()
{
	if (Cast<ABaseSpaceship>(GetPawn()))
	{
		ControlledShip = Cast<ABaseSpaceship>(GetPawn());
		bIsControllingShip = true;
		ControlledShip->bIsAIControlled = false;
	}
}