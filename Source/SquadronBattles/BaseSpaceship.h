// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BaseSpaceship.generated.h"

UCLASS()
class SQUADRONBATTLES_API ABaseSpaceship : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseSpaceship();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
	float Mass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float MAX_FORCE;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float MAX_VELOCITY;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float SLOWING_RADIUS;



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float PitchIncrement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float RollIncrement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float YawIncrement;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Squadron")
	ABaseSpaceship* Leader;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float ThrustIncrement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	float MaxThrust;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void AddPitch(float Magnitude);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void AddRoll(float Magnitude);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void AddYaw(float Magnitude);


	UFUNCTION(BlueprintCallable, Category = "Movement")
	void AddThrust(float Magnitude);


	UFUNCTION(BlueprintCallable, Category = "Squadron")
	bool CheckIsLeader();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetMovementTarget();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	FVector GetDesiredVelocity();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	FVector GetCurrentVelocity();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	FVector GetSteering();


	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTargetLocation(FVector Target);


	bool bIsAIControlled;

private:
	
	FRotator TargetRotation;
	float TargetThrust;


	FVector Truncate(FVector Value, float Max);
	FVector Seek(FVector Target);
	FVector Arrive(FVector Target);

	FVector TargetLocation;
	FVector DesiredVelocity;
	FVector CurrentVelocity;
	FVector Steering;
	

};
