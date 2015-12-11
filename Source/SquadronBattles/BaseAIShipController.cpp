// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
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

void ABaseAIShipController::Tick(float DeltaSeconds)
{
	if (!ControlledShip->CheckIsLeader())
	{
		/*
		FVector LeaderForward = ControlledShip->Leader->GetActorForwardVector();
		LeaderForward = -LeaderForward;

		FVector TargetLocation = ControlledShip->Leader->GetActorLocation() + LeaderForward * 50.0f;

		
		
		*/

		TargetRotation = FRotationMatrix::MakeFromX(TargetLocation - ControlledShip->GetActorLocation()).Rotator();
		ControlledShip->SetTargetRotation(TargetRotation);

		float DistanceToTarget = FVector::Dist(TargetLocation, ControlledShip->GetActorLocation());


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
