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
}

void ABaseShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseShip, CurrentForwardSpeed);
	DOREPLIFETIME(ABaseShip, CurrentTurningSpeed);
}

// Called every frame
void ABaseShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.0f, 0.0f);

	FRotator DeltaRotation;
	DeltaRotation.Pitch = CurrentTurningSpeed.Pitch * DeltaSeconds;
	DeltaRotation.Yaw = CurrentTurningSpeed.Yaw * DeltaSeconds;
	DeltaRotation.Roll = CurrentTurningSpeed.Roll *DeltaSeconds;

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
	UE_LOG(LogTemp, Warning, TEXT("HITTING SOMETHING"));
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Set velocity to zero upon collision
	CurrentForwardSpeed = 0.0f;
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


void ABaseShip::FireWeapon()
{
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			ABaseProjectile* const Projectile = World->SpawnActor<ABaseProjectile>(ProjectileClass, GetActorLocation() + GetActorForwardVector() * FiringOffset, GetActorRotation(), SpawnParams);
			if (Projectile)
			{
				Projectile->InitVelocity(GetActorForwardVector());
			}
		}
	}
}
