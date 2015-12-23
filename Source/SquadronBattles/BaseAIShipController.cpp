// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseSpaceship.h"
#include "BaseAIShipController.h"




ABaseAIShipController::ABaseAIShipController()
{
	ThrustMultiplier = 0.25f;
	TargetLocation = FVector::ZeroVector;
	TargetRotation = FRotator::ZeroRotator;
}

void ABaseAIShipController::BeginPlay()
{
	if (Cast<ABaseSpaceship>(GetPawn()))
	{
		ControlledShip = Cast<ABaseSpaceship>(GetPawn());
		UE_LOG(LogTemp, Warning, TEXT("Casting to pawn"));

		ControlledShip->bIsAIControlled = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Casting to pawn"));
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
}

/*
void ABaseAIShipController::Tick(float DeltaSeconds)
{
	if (!ControlledShip->CheckIsLeader())
	{
		TargetRotation = FRotationMatrix::MakeFromX(TargetLocation - ControlledShip->GetActorLocation()).Rotator();
		TargetRotation.Roll = ControlledShip->Leader->GetActorRotation().Roll;
		ControlledShip->SetTargetRotation(TargetRotation);

		float DistanceToTarget = FVector::Dist(TargetLocation, ControlledShip->GetActorLocation());

		FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(TargetLocation, ControlledShip->GetActorLocation());

		if ((DistanceToTarget < 50) && ((FMath::Abs(RotationToTarget.Pitch) > 45) || (FMath::Abs(RotationToTarget.Yaw) > 45)))
		{
			ControlledShip->ThrustOverride = 0.0f;
			return;
		}

		FColor PointColor;

		if (DistanceToTarget < 300.0f)
		{
			ControlledShip->ThrustOverride = ControlledShip->Leader->GetCurrentThrust();
			PointColor = FColor::Green;
		}
		else
		{
			ControlledShip->ThrustOverride = ControlledShip->MaxThrust;
			PointColor = FColor::Red;
		}

		DrawDebugPoint(GetWorld(), TargetLocation, 20, PointColor);
	}
}
*/

void ABaseAIShipController::Tick(float DeltaSeconds)
{
	if (!ControlledShip->CheckIsLeader())
	{
		ControlledShip->SetTargetLocation(TargetLocation);

		//UE_LOG(LogTemp, Warning, TEXT("Target Location : %s"), *TargetLocation.ToString());

		float DistanceToTarget = FVector::Dist(TargetLocation, ControlledShip->GetActorLocation());

		FColor PointColor;

		if (DistanceToTarget < 300.0f)
		{
			PointColor = FColor::Green;
		}
		else
		{
			PointColor = FColor::Red;
		}

		DrawDebugPoint(GetWorld(), TargetLocation, 20, PointColor);
	}
}
