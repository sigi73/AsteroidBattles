// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "BaseShip.h"


// Sets default values
ABaseShip::ABaseShip()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/LowPolyShip/lowPoly.lowPoly"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	RootComponent = PlaneMesh;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller


 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Acceleration = 500.0f;
	TurnSpeed = 150.0f;
	MaxSpeed = 4000.0f;
	MinSpeed = 500.0f;
	CurrentForwardSpeed = 500.0f;
}

// Called every frame
void ABaseShip::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.0f, 0.0f);

	AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation;
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	AddActorLocalRotation(DeltaRotation);
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

	// When steering, we decrease pitch slightly
	//TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void ABaseShip::HorizontalInput(float Magnitude)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Magnitude * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Magnitude) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.0f);
	TargetRollSpeed = 0.0f;

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void ABaseShip::RollInput(float Magnitude)
{
	float TargetRollSpeed = (Magnitude * TurnSpeed);

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}