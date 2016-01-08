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
	//ClientMessage(TEXT("Ticking"));
	//ClientMessage(bIsControllingShip ? TEXT("true") : TEXT("false"));
	if (!bIsControllingShip)
	{
		ClientMessage(TEXT("Not yet controlling ship"));
		UE_LOG(LogTemp, Warning, TEXT("Not yet controlling ship"));
		TakeControlOfShip();
	}
	else
	{
		//ControlledShip->SetOwner(this);
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
	FString RoleValue;
	if (Role == ROLE_Authority)
		RoleValue = "ROLE_Authority";
	else if (Role == ROLE_AutonomousProxy)
		RoleValue = "ROLE_AutonomousProxy";
	else if (Role == ROLE_SimulatedProxy)
		RoleValue = "Role_SimulatedProxy";
	else
		RoleValue = "None";


	//UE_LOG(LogTemp, Warning, TEXT("Role: %s"), *RoleValue);
	ClientMessage(RoleValue);
}

void AShipController::TakeControlOfShip()
{
	//ClientMessage(TEXT("Taking control of ship"));
	//UE_LOG(LogTemp, Warning, TEXT("Taking control of ship"));
	if (Cast<ABaseSpaceship>(GetPawn()))
	{
		//ClientMessage(TEXT("Cast succeeded"));
		ControlledShip = Cast<ABaseSpaceship>(GetPawn());
		//UE_LOG(LogTemp, Warning, TEXT("Pre bIsControllingShip: %s"), bIsControllingShip ? TEXT("true") : TEXT("false"));
		bIsControllingShip = true;
		//UE_LOG(LogTemp, Warning, TEXT("Post bIsControllingShip: %s"), bIsControllingShip ? TEXT("true") : TEXT("false"));
		ControlledShip->bIsAIControlled = false;
		ControlledShip->SetOwner(this);

		//ClientMessage(ControlledShip->GetOwner()->GetName());
	}
}

void AShipController::FireWeapon()
{
	if (bIsControllingShip)
		ControlledShip->FireWeapon();
}