// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "BezierLoopGenerator.h"



void UBezierLoopGenerator::GenerateBezierLoop()
{
	GenerateCurvePoints();
}

void UBezierLoopGenerator::GenerateCurvePoints()
{
	if (PointNum % 2 != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Point Num not even"));
		return;
	}

	for (int i = 0; i < PointNum; i++)
	{
		float SineVal, CosVal, Val;
		Val = i * 2.0f * PI / (float)PointNum;
		SineVal = FMath::Sin(Val);
		CosVal = FMath::Cos(Val);
		CurvePoints.Add(FVector(SineVal * Radius, CosVal * Radius, 0.0f));

		UE_LOG(LogTemp, Warning, TEXT("Point %d is at %f and %f"), i, SineVal, CosVal);
	}
}
