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
class SQUADRONBATTLES_API UBezierLoopGenerator : public UObject
{
	GENERATED_BODY()
	
public:

	void GenerateBezierLoop();

	TArray<FVector> BezierPoints;

private:

	TArray<FVector> CurvePoints;
	TArray<FVector> CurveTangents;

	int32 PointNum;
	float Radius;

	float TimeStep;

	void GenerateCurvePoints();

};
