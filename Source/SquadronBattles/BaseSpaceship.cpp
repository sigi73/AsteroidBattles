// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnrealNetwork.h"
#include "BaseProjectile.h"
#include "BaseSpaceship.h"


//http://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-seek--gamedev-849

// Sets default values
ABaseSpaceship::ABaseSpaceship()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Mass = 10.0f;

	MAX_FORCE = 10.0f;
	MAX_VELOCITY = 10.0f;
	SLOWING_RADIUS = 10.0f;

	TargetLocation = FVector(0.0f, 0.0f, 0.0f);
	DesiredVelocity = FVector(0.0f, 0.0f, 0.0f);
	CurrentVelocity = FVector(0.0f, 0.0f, 0.0f);
	Steering = FVector(0.0f, 0.0f, 0.0f);

	ThrustIncrement = 1.0f;
	MaxThrust = 10.0f;

	PitchIncrement = 10.0f;
	RollIncrement = 10.0f;
	YawIncrement = 10.0f;

	bIsAIControlled = false;

	TargetThrust = 0.0f;
	TargetRotation = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void ABaseSpaceship::BeginPlay()
{
	if (Leader == NULL)
		Leader = this;

	SetOwner(GetController());
	//UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *GetOwner()->GetName());

	Super::BeginPlay();
}

// Called every frame
void ABaseSpaceship::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


	CalculateSteering();

	SetCurrentVelocity();

	MoveShip(CurrentVelocity);
}

void ABaseSpaceship::CalculateSteering()
{
	if (bIsAIControlled)
	{
		Steering = Arrive(TargetLocation);
	}
	else
	{
		FVector RotatedVector = (GetActorRotation() + TargetRotation).Vector();
		if (CurrentVelocity == FVector::ZeroVector)
			RotatedVector = GetActorRotation().Vector();

		Steering = Seek(GetActorLocation() + RotatedVector * TargetThrust);

		TargetRotation = FRotator::ZeroRotator;
	}

	Steering = Truncate(Steering, MAX_FORCE);
	Steering /= Mass;
}

void ABaseSpaceship::SetCurrentVelocity()
{
	CurrentVelocity += Steering;
	CurrentVelocity = Truncate(CurrentVelocity, MAX_VELOCITY);

	if (CurrentVelocity.Size() < 1.0f)
	{
		CurrentVelocity = FVector::ZeroVector;
	}
}

void ABaseSpaceship::MoveShip(FVector InputVelocity)
{
	if (Role == ROLE_AutonomousProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server move ship is being called"));
		ServerMoveShip(InputVelocity);	
	}

	SetActorLocation(GetActorLocation() + InputVelocity, true);

	if (InputVelocity != FVector::ZeroVector)
	{
		FRotator CurrentRotation = InputVelocity.Rotation();
		SetActorRotation(CurrentRotation);
	}
}

void ABaseSpaceship::ServerMoveShip_Implementation(FVector InputVelocity)
{
	MoveShip(InputVelocity);
}

bool ABaseSpaceship::ServerMoveShip_Validate(FVector InputVelocity)
{
	return true;
}

// Called to bind functionality to input
void ABaseSpaceship::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}


void ABaseSpaceship::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseSpaceship, CurrentVelocity);
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
	{
		return FVector::ZeroVector;
	}

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

void ABaseSpaceship::FireWeapon()
{
	if (Role == ROLE_AutonomousProxy)
	{
		ServerFireWeapon();
	}
	else
	{

		if (ProjectileClass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				ABaseProjectile* const Projectile = World->SpawnActor<ABaseProjectile>(ProjectileClass, GetActorLocation() + FiringOffset, GetActorRotation(), SpawnParams);
				if (Projectile)
				{
					Projectile->InitVelocity(GetActorForwardVector());
				}
			}
		}
	}
}

void ABaseSpaceship::ServerFireWeapon_Implementation()
{
	FireWeapon();
}

bool ABaseSpaceship::ServerFireWeapon_Validate()
{
	return false;
}