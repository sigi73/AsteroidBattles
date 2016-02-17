// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BaseShip.generated.h"

UCLASS()
class SQUADRONBATTLES_API ABaseShip : public APawn
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	// Sets default values for this pawn's properties
	ABaseShip();

	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal,
		FVector NormalImpulse, const FHitResult& Hit) override;

	void ThrustInput(float Magnitude);
	void VerticalInput(float Magnitude);
	void HorizontalInput(float Magnitude);
	void RollInput(float Magnitude);

protected:
	UPROPERTY(Category = "Movement", EditAnywhere)
	float Acceleration;

	UPROPERTY(Category = "Movement", EditAnywhere)
	float TurnSpeed;

	UPROPERTY(Category = "Movement", EditAnywhere)
	float MaxSpeed;

	UPROPERTY(Category = "Movement", EditAnywhere)
	float MinSpeed;

	float CurrentForwardSpeed;

	float CurrentYawSpeed;
	float CurrentPitchSpeed;
	float CurrentRollSpeed;


};
