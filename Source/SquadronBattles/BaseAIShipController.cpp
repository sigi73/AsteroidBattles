// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseShip.h"
#include "BaseAIShipController.h"




ABaseAIShipController::ABaseAIShipController()
{
	ThrustMultiplier = 0.25f;
	TargetLocation = FVector::ZeroVector;
	TargetRotation = FRotator::ZeroRotator;
}

void ABaseAIShipController::BeginPlay()
{
	ControlledShip = Cast<ABaseShip>(GetPawn());
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
		FVector CurrentLocation = GetPawn()->GetActorLocation();
		FVector2D Vertex = FVector2D(CurrentLocation.X, CurrentLocation.Y);

		FVector2D Target2D = FVector2D(TargetLocation.X, TargetLocation.Y);

		float quadraticA = ((Target2D.Y - Vertex.Y) / (FMath::Square(Target2D.X - Vertex.Y)));


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
