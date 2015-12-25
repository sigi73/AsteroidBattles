// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnrealNetwork.h"
#include "BaseSpaceship.h"


//http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-seek--gamedev-849

// Sets default values
ABaseSpaceship::ABaseSpaceship()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	MAX_FORCE = 1.0f;
	MAX_VELOCITY = 1.0f;
	SLOWING_RADIUS = 10.0f;

	TargetLocation = FVector(0.0f, 0.0f, 0.0f);
	DesiredVelocity = FVector(0.0f, 0.0f, 0.0f);
	CurrentVelocity = FVector(0.0f, 0.0f, 0.0f);
	Steering = FVector(0.0f, 0.0f, 0.0f);

	ThrustIncrement = 1.0f;
	MaxThrust = 10.0f;

	PitchIncrement = 1.0f;
	RollIncrement = 1.0f;
	YawIncrement = 1.0f;

	bIsAIControlled = false;
}

// Called when the game starts or when spawned
void ABaseSpaceship::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseSpaceship::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	

	float DeltaRoll = 0.0f;

	if (bIsAIControlled)
	{
		Steering = Arrive(TargetLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Seeking"));
	}
	else
	{
		FVector RotatedVector = (GetActorRotation() + TargetRotation).Vector();
		DeltaRoll = TargetRotation.Roll;
		Steering = Seek(GetActorLocation() + RotatedVector * TargetThrust);

		TargetRotation = FRotator::ZeroRotator;
	}

	Steering = Truncate(Steering, MAX_FORCE);
	Steering /= Mass;

	CurrentVelocity += Steering;
	CurrentVelocity = Truncate(CurrentVelocity, MAX_VELOCITY);

	//if (bIsAIControlled) UE_LOG(LogTemp, Warning, TEXT("CurrentVelocity: %s, %f"), *CurrentVelocity.ToString(), CurrentVelocity.Size());
	if (CurrentVelocity.Size() < 1.0f)
	{
		//if (bIsAIControlled) UE_LOG(LogTemp, Warning, TEXT("Is Nearly 0"));
		CurrentVelocity = FVector::ZeroVector;
	}

	SetActorLocation(GetActorLocation() + CurrentVelocity);

	FRotator CurrentRotation = CurrentVelocity.Rotation();
	//CurrentRotation.Roll += DeltaRoll;

	/*
	if (DeltaRoll != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Delta roll: %f"), DeltaRoll);
	}
	*/

	SetActorRotation(CurrentRotation);
}

// Called to bind functionality to input
void ABaseSpaceship::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}


void ABaseSpaceship::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseSpaceship, Leader);
	DOREPLIFETIME(ABaseSpaceship, TargetRotation);
	DOREPLIFETIME(ABaseSpaceship, TargetThrust);
	DOREPLIFETIME(ABaseSpaceship, TargetLocation);
	DOREPLIFETIME(ABaseSpaceship, DesiredVelocity);
	DOREPLIFETIME(ABaseSpaceship, CurrentVelocity);
	DOREPLIFETIME(ABaseSpaceship, Steering);
}


FVector ABaseSpaceship::Truncate(FVector Value, float Max)
{
	//return (Value.Size() > Max) ? (Value.GetSafeNormal() * Max) : Value;
	if (Value.Size() > Max)
	{
		return Value.GetSafeNormal() * Max;
	}
	else
	{
		return Value;
	}
}

FVector ABaseSpaceship::Seek(FVector Target)
{
	if ((Target - GetActorLocation()).IsNearlyZero())
		return FVector::ZeroVector;

	DesiredVelocity = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target).Vector();
	//DesiredVelocity = Target - GetActorLocation();
	DesiredVelocity.Normalize();
	DesiredVelocity *= MAX_VELOCITY;

	FVector Force = DesiredVelocity - CurrentVelocity;

	return Force;
}

FVector ABaseSpaceship::Arrive(FVector Target)
{
	if ((Target - GetActorLocation()).IsNearlyZero(5.0f))
		return FVector::ZeroVector;

	DesiredVelocity = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target).Vector();
	float DistanceToTarget = DesiredVelocity.Size();
	DesiredVelocity.Normalize();

	if (DistanceToTarget < SLOWING_RADIUS)
	{
		DesiredVelocity *= (MAX_VELOCITY * (DistanceToTarget / SLOWING_RADIUS));
	}
	else
	{
		DesiredVelocity *= MAX_VELOCITY;
	}

	FVector Force = DesiredVelocity - CurrentVelocity;

	return Force;
}



void ABaseSpaceship::AddPitch(float Magnitude)
{
	TargetRotation.Pitch += Magnitude * PitchIncrement;
	if (Magnitude == 0.0f)
		TargetRotation.Pitch = GetActorRotation().Pitch;
}

void ABaseSpaceship::AddRoll(float Magnitude)
{
	TargetRotation.Roll += Magnitude * RollIncrement;

	if (Magnitude == 0.0f)
		TargetRotation.Roll = GetActorRotation().Roll;
}

void ABaseSpaceship::AddYaw(float Magnitude)
{
	TargetRotation.Yaw += Magnitude * YawIncrement;

	if (Magnitude == 0.0f)
		TargetRotation.Yaw = GetActorRotation().Yaw;
}


void ABaseSpaceship::AddThrust(float Magnitude)
{
	TargetThrust += Magnitude * ThrustIncrement;

	TargetThrust = FMath::Clamp(TargetThrust, -MaxThrust, MaxThrust);
}

bool ABaseSpaceship::CheckIsLeader()
{
	if (Leader->GetUniqueID() == GetUniqueID())
		return true;
	else
		return false;
}

FVector ABaseSpaceship::GetMovementTarget()
{
	return TargetLocation;
}

FVector ABaseSpaceship::GetDesiredVelocity()
{
	return DesiredVelocity;
}

FVector ABaseSpaceship::GetCurrentVelocity()
{
	return CurrentVelocity;
}

FVector ABaseSpaceship::GetSteering()
{
	return Steering;
}


void ABaseSpaceship::SetTargetLocation(FVector Target)
{
	TargetLocation = Target;
}