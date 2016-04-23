// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "BezierLoopGenerator.h"




void UBezierLoopGenerator::GenerateBezierLoop()
{
	GenerateCurvePoints();
	GenerateTangentPoints();
	GenerateCurve();
}

void UBezierLoopGenerator::GenerateCurvePoints()
{
	if (PointNum % 2 != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Point Num not even"));
		return;
	}
	CurvePoints.Empty();

	for (int i = 0; i < PointNum; i++)
	{
		float SineVal, CosVal, Val;
		Val = i * 2.0f * PI / (float)PointNum;
		SineVal = FMath::Sin(Val);
		CosVal = FMath::Cos(Val);

		float RandDist = Radius + FMath::RandRange(-DeltaRadius, DeltaRadius);

		CurvePoints.Add(FVector(SineVal * RandDist, CosVal * RandDist, 0.0f));

		//UE_LOG(LogTemp, Warning, TEXT("Point %d is at %f and %f"), i, SineVal, CosVal);
	}
}


void UBezierLoopGenerator::GenerateTangentPoints()
{
	IncomingCurveTangents.Empty();
	OutgoingCurveTangents.Empty();

	IncomingCurveTangents.AddUninitialized(CurvePoints.Num());
	OutgoingCurveTangents.AddUninitialized(CurvePoints.Num());

	for (int i = 0; i < CurvePoints.Num(); i++)
	{
		/*
		FVector Point = CurvePoints[i];
		FVector Tangent = RandomTangentAtDist(Point, 100.0f);
		OutgoingCurveTangents[i] = Tangent;
		IncomingCurveTangents[i] = Tangent;
		*/
		
		FVector Point = FVector::ZeroVector;
		FVector Tangent = RandomTangentAtDist(Point, 100.0f);

		FVector Pos = CurvePoints[i] + Tangent;
		FVector Neg = CurvePoints[i] - Tangent;

		if (i > 0)
		{
			if (FVector::DistSquared(Pos, CurvePoints[i - 1]) < FVector::DistSquared(Neg, CurvePoints[i - 1]))
			{
				IncomingCurveTangents[i] = Neg;
				OutgoingCurveTangents[i] = Pos;
			}
			else
			{
				IncomingCurveTangents[i] = Pos;
				OutgoingCurveTangents[i] = Neg;
			}
		}

		OutgoingCurveTangents[i] = CurvePoints[i] - Tangent;
		IncomingCurveTangents[i] = CurvePoints[i] + Tangent;
	}
}

FVector UBezierLoopGenerator::RandomTangentAtDist(FVector Point, float Dist)
{
	float Angle = FMath::RandRange(0.0f, PI / 2.0f);

	return FVector(Point.X + FMath::Cos(Angle) * Dist, Point.Y + FMath::Sin(Angle) * Dist, Point.Z);
}

void UBezierLoopGenerator::GenerateCurve()
{
	for (int i = 1; i < CurvePoints.Num(); i++)
	{
		FVector ControlPoints[4];
		ControlPoints[0] = CurvePoints[i - 1];
		ControlPoints[1] = OutgoingCurveTangents[i - 1];
		ControlPoints[2] = IncomingCurveTangents[i];
		ControlPoints[3] = CurvePoints[i];
		TArray<FVector> Points;
		FVector::EvaluateBezier(ControlPoints, 1.0f / TimeStep, Points);

		BezierPoints.Append(Points);
	}
	FVector ControlPoints[4];
	ControlPoints[0] = CurvePoints[CurvePoints.Num() - 1];
	ControlPoints[1] = OutgoingCurveTangents[CurvePoints.Num() - 1];
	ControlPoints[2] = IncomingCurveTangents[0];
	ControlPoints[3] = CurvePoints[0];
	TArray<FVector> Points;
	FVector::EvaluateBezier(ControlPoints, 1.0f / TimeStep, Points);

	BezierPoints.Append(Points);
}

FVector UBezierLoopGenerator::QuadraticCurve(float t, FVector P0, FVector P1, FVector P2)
{
	return (1.0f - t) * (1.0f - t) * P0 + 2.0f * (1.0f - t) * t * P1 + t * t * P2;
}

FVector UBezierLoopGenerator::CubicCurve(float t, FVector P0, FVector P1, FVector P2, FVector P3)
{
	return (1.0f - t) * QuadraticCurve(t, P0, P1, P2) + t * QuadraticCurve(t, P1, P2, P3);
}

void UBezierLoopGenerator::DebugTangents()
{
	for (int i = 0; i < CurvePoints.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s %s %s"), *CurvePoints[i].ToString(), *IncomingCurveTangents[i].ToString(), *OutgoingCurveTangents[i].ToString());
	}
}