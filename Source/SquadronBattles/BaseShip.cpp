// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "BaseProjectile.h"
#include "UnrealNetwork.h"
#include "BaseShip.h"


// Sets default values
ABaseShip::ABaseShip()
{
	bReplicates = true;
	bReplicateMovement = true;
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Acceleration = 500.0f;
	TurnSpeed = 150.0f;
	MaxSpeed = 4000.0f;
	MinSpeed = 0.0f;
	CurrentForwardSpeed = 0.0f;
	Health = 20;
	CollisionDampFactor = 1.0f;
	BounceFactor = 1.0f;

	TempDateTime = FDateTime();
}

void ABaseShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseShip, CurrentForwardSpeed);
	DOREPLIFETIME(ABaseShip, CurrentTurningSpeed);
	DOREPLIFETIME(ABaseShip, Health);
}

// Called every frame
void ABaseShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.0f, 0.0f);

	FRotator DeltaRotation;
	DeltaRotation.Pitch = CurrentTurningSpeed.Pitch * DeltaSeconds;
	DeltaRotation.Yaw = CurrentTurningSpeed.Yaw * DeltaSeconds;
	DeltaRotation.Roll = CurrentTurningSpeed.Roll * DeltaSeconds;

	MoveShip(LocalMove, DeltaRotation);
}

void ABaseShip::MoveShip(FVector LocationOffset, FRotator RotationOffset)
{
	if (Role == ROLE_AutonomousProxy)
	{
		ServerMoveShip(LocationOffset, RotationOffset);
	}
	AddActorLocalOffset(LocationOffset, true);
	AddActorLocalRotation(RotationOffset);
}

void ABaseShip::ServerMoveShip_Implementation(FVector LocationOffset, FRotator RotationOffset)
{
	MoveShip(LocationOffset, RotationOffset);
}

bool ABaseShip::ServerMoveShip_Validate(FVector LocationOffset, FRotator RotationOffset)
{
	return true;
}

void ABaseShip::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Set velocity to zero upon collision
	
	//CurrentForwardSpeed = 0.0f;

	
	//FVector NewForward = FVector::VectorPlaneProject(GetActorForwardVector(), HitNormal);
	//FRotator NewRotation = NewForward.Rotation();
	//NewRotation.Roll = GetActorRotation().Roll;
	//CurrentTurningSpeed = (NewRotation - GetActorRotation()).GetNormalized() * CollisionTurnFactor;

	Bounce(HitNormal, BounceFactor);
}

void ABaseShip::Bounce(FVector Direction, float Magnitude)
{
	if (Role == ROLE_AutonomousProxy)
	{
		ServerBounce(Direction, Magnitude);
	}
	AddActorLocalOffset(Direction * Magnitude);
	CurrentForwardSpeed = CurrentForwardSpeed * CollisionDampFactor;
}

void ABaseShip::ServerBounce_Implementation(FVector Direction, float Magnitude)
{
	Bounce(Direction, Magnitude);
}

bool ABaseShip::ServerBounce_Validate(FVector Direction, float Magnitude)
{
	return true;
}



void ABaseShip::ThrustInput(float Magnitude)
{
	bool bHasInput = !FMath::IsNearlyEqual(Magnitude, 0.0f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Magnitude * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ABaseShip::VerticalInput(float Magnitude)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Magnitude * TurnSpeed);

	// Smoothly interpolate to target pitch speed
	CurrentTurningSpeed.Pitch = FMath::FInterpTo(CurrentTurningSpeed.Pitch, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void ABaseShip::HorizontalInput(float Magnitude)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Magnitude * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentTurningSpeed.Yaw = FMath::FInterpTo(CurrentTurningSpeed.Yaw, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void ABaseShip::RollInput(float Magnitude)
{
	float TargetRollSpeed = (Magnitude * TurnSpeed);

	CurrentTurningSpeed.Roll = FMath::FInterpTo(CurrentTurningSpeed.Roll, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}



void ABaseShip::ServerFireWeapon_Implementation()
{
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FRotator WeaponDirection = (FiringTarget - (GetActorLocation() + GetActorForwardVector() * FiringOffset)).Rotation();

			ABaseProjectile* const Projectile = World->SpawnActor<ABaseProjectile>(ProjectileClass, GetActorLocation() + GetActorForwardVector() * FiringOffset, WeaponDirection, SpawnParams);
			if (Projectile)
			{
				Projectile->InitVelocity(WeaponDirection.Vector());
			}
		}
	}
}

bool ABaseShip::ServerFireWeapon_Validate()
{
	return true;
}


void ABaseShip::ApplyDamage_Implementation(float Amount)
{
	if (Role < ROLE_Authority)
	{
		ServerApplyDamage(Amount);
	}
	else
	{
		APlayerController* OwnController = Cast<APlayerController>(GetController());

		if (OwnController)
		{
			OwnController->ClientMessage("Getting hit");
		}
		
		Health -= Amount;
		if (Health <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Me ded"));
		}
		
	}
	
}

void ABaseShip::ServerApplyDamage_Implementation(float Amount)
{
	Execute_ApplyDamage(this, Amount);
}

bool ABaseShip::ServerApplyDamage_Validate(float Amount)
{
	return true;
}
