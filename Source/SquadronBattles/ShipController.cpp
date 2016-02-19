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
	UE_LOG(LogTemp, Warning, TEXT("Beginning of begin play"));
	ClientMessage(TEXT("Beginning of begin play"));
	TakeControlOfShip();
	UE_LOG(LogTemp, Warning, TEXT("End of begin play"));
	ClientMessage(TEXT("End of begin play"));
}

void AShipController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Fire", IE_Pressed, this, &AShipController::FireWeapon);

	InputComponent->BindAxis("Thrust", this, &AShipController::AddThrust);
	InputComponent->BindAxis("Roll", this, &AShipController::AddRoll);
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


void AShipController::Tick(float DeltaSeconds)
{
	if (!bIsControllingShip)
	{
		ClientMessage(TEXT("Not yet controlling ship"));
		UE_LOG(LogTemp, Warning, TEXT("Not yet controlling ship"));
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
		ControlledShip->FireWeapon();
}