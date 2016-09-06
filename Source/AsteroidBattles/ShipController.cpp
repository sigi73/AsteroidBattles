// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidBattles.h"
#include "ICursor.h"
#include "IHeadMountedDisplay.h"
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
	Super::BeginPlay();
	
	TakeControlOfShip();
	
}

void AShipController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Fire", IE_Pressed, this, &AShipController::FireWeapon);
	InputComponent->BindAction("Fire", IE_Released, this, &AShipController::StopFiring);

	InputComponent->BindAxis("Thrust", this, &AShipController::AddThrust);
	InputComponent->BindAxis("Roll", this, &AShipController::AddRoll);
	InputComponent->BindAxis("Vertical", this, &AShipController::VerticalInput);
	InputComponent->BindAxis("Horizontal", this, &AShipController::HorizontalInput);
}

void AShipController::AddThrust(float Magnitude)
{
	if (bIsControllingShip)
	{
		ControlledShip->ThrustInput(Magnitude);
	}
}


void AShipController::AddPitch(float Magnitude)
{
	if (bIsControllingShip)
	{
		ControlledShip->VerticalInput(Magnitude);
	}
}

void AShipController::AddRoll(float Magnitude)
{
	if (bIsControllingShip)
	{
		ControlledShip->RollInput(Magnitude);
	}
}

void AShipController::AddYaw(float Magnitude)
{
	if (bIsControllingShip)
	{
		ControlledShip->HorizontalInput(Magnitude);
	}
}


void AShipController::VerticalInput(float Magnitude)
{
	if (bIsControllingShip)
	{
		if (FMath::Abs(Magnitude) > MouseInputDelta)
		{
			AddPitch(Magnitude * MouseDeltaMultiplier);
		}
		else
		{
			AddPitch(0);
		}
	}
}

void AShipController::HorizontalInput(float Magnitude)
{
	if (bIsControllingShip)
	{
		if (FMath::Abs(Magnitude) > MouseInputDelta)
		{
			AddYaw(Magnitude * MouseDeltaMultiplier);
		}
		else
		{
			AddYaw(0);
		}
	}
}

void AShipController::Tick(float DeltaSeconds)
{
	if (!bIsControllingShip)
	{
		TakeControlOfShip();
	}
	/*
	if (!bIsControllingShip)
	{
		TakeControlOfShip();
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
	*/
}

void AShipController::TakeControlOfShip()
{
	if (Cast<ABaseShip>(GetPawn()))
	{
		ControlledShip = Cast<ABaseShip>(GetPawn());
		bIsControllingShip = true;
		ControlledShip->SetOwner(this);		
	}
}

void AShipController::FireWeapon()
{
	if (bIsControllingShip)
		ControlledShip->ShouldFireWeapon();
}

void AShipController::StopFiring()
{
	if (bIsControllingShip)
		ControlledShip->ShouldStopFiringWeapon();
}