// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "BezierLoopGenerator.generated.h"

USTRUCT(BlueprintType)
struct FTri
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Point1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Point2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Point3;
};

/**
 * 
 */
UCLASS()
class ASTEROIDBATTLES_API UBezierLoopGenerator : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Map Generation")
	void GenerateBezierLoop();

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugTangents();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> BezierPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PointNum;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeltaRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> CurvePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> IncomingCurveTangents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> OutgoingCurveTangents;

private:

	

	

	void GenerateCurvePoints();
	void GenerateTangentPoints();
	void GenerateCurve();

	FVector QuadraticCurve(float t, FVector P0, FVector P1, FVector P2);
	FVector CubicCurve(float t, FVector P0, FVector P1, FVector P2, FVector P3);

	FVector RandomTangentAtDist(FVector Point, float Dist);
	FVector TangentColinear(FVector Point, FVector Tangent, float Dist);
};
