// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "BaseShip.h"


// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RotationInterpSpeed = 1.0f;
	ThrustInterpSpeed = 1.0f;
	CurrentThrust = 0.0f;

	PitchIncrement = 1.0f;
	RollIncrement = 1.0f;
	YawIncrement = 1.0f;


	ThrustIncrement = 50.0f;
	MaxThrust = 1000.0f;

	DefaultRoll = 0.0f;

	ThrustOverride = -1.0f;
}

// Called when the game starts or when spawned
void ABaseShip::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator CurrentRotation = GetActorRotation();

	TargetRotation.Normalize();

	CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationInterpSpeed);
	CurrentThrust = FMath::FInterpTo(CurrentThrust, TargetThrust, DeltaSeconds, RotationInterpSpeed);

	FRotator DeltaRotation = CurrentRotation - GetActorRotation();

	AddActorLocalRotation(DeltaRotation.GetNormalized());

	if (ThrustOverride == -1.0f)
		AddActorLocalOffset(FVector(CurrentThrust, 0.0f, 0.0f) * DeltaSeconds, true);
	else
		AddActorLocalOffset(FVector(ThrustOverride, 0.0f, 0.0f) * DeltaSeconds, true);
}

// Called to bind functionality to input
void ABaseShip::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ABaseShip::AddPitch(float Magnitude)
{
	TargetRotation.Pitch += Magnitude * PitchIncrement;
	if (Magnitude == 0.0f)
		TargetRotation.Pitch = GetActorRotation().Pitch;
}

void ABaseShip::AddRoll(float Magnitude)
{
	TargetRotation.Roll += Magnitude * RollIncrement;

	if (Magnitude == 0.0f)
		TargetRotation.Roll = GetActorRotation().Roll;
}

void ABaseShip::AddYaw(float Magnitude)
{
	TargetRotation.Yaw += Magnitude * YawIncrement;
	if (Magnitude == 0.0f)
		TargetRotation.Yaw = GetActorRotation().Yaw;
}


void ABaseShip::AddThrust(float Magnitude)
{
	TargetThrust += Magnitude * ThrustIncrement;

	TargetThrust = FMath::Clamp(TargetThrust, 0.0f, MaxThrust);
}


void ABaseShip::SetTargetRotation(FRotator Target)
{
	TargetRotation = Target;
}

bool ABaseShip::CheckIsLeader()
{
	if (Leader->GetUniqueID() == GetUniqueID())
		return true;
	else
		return false;
}

void ABaseShip::SetTargetThrust(float Target)
{
	TargetThrust = Target;
	TargetThrust = FMath::Clamp(TargetThrust, 0.0f, MaxThrust);
}

float ABaseShip::GetCurrentThrust()
{
	return CurrentThrust;
}