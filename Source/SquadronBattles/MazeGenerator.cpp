// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadronBattles.h"
#include "MazeGenerator.h"




void AMazeGenerator::GenerateMaze(int32 NewMinEnd, int32 NewMaxEnd, float NewDistMin, float NewDistMax, FVector NewStart)
{
	EndPoints.Empty();
	EndPointDirections.Empty();
	Passages.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Working"));
	MinEndPoints = NewMinEnd;
	MaxEndPoints = NewMaxEnd;
	DistMin = NewDistMin;
	DistMax = NewDistMax;
	Start = NewStart;

	EndPoints.Add(Start);
	EndPointDirections.Add(EDirectionEnum::DE_BACKWARD);


	while (EndPoints.Num() < MinEndPoints)
	{
		UE_LOG(LogTemp, Warning, TEXT("Num: %d"), EndPoints.Num())
		AddEndPoints();
	}
	UE_LOG(LogTemp, Warning, TEXT("Ended"));
}

void AMazeGenerator::AddEndPoints()
{
	TArray<FVector> TempPoints;
	TArray<EDirectionEnum> TempDirections;
	for (FVector Point : EndPoints)
	{
		if (EndPoints.Num() > MaxEndPoints)
			break;

		TArray<FVector> NewEndPoints;

		TArray<EDirectionEnum> Exclude;
		Exclude.Add(EndPointDirections[EndPoints.IndexOfByKey(Point)]);
		int32 DirectionNum = FMath::RandRange(1, 3);

		for (int i = 0; i < DirectionNum; i++)
		{
			EDirectionEnum Direction = ChooseRandomDirection(Exclude);
			Exclude.Add(Direction);
			
			FVector VectorDirection;

			switch (Direction)
			{
				case EDirectionEnum::DE_FORWARD:
					VectorDirection = FVector(1, 0, 0);
					break;
				
				case EDirectionEnum::DE_BACKWARD:
					VectorDirection = FVector(-1, 0, 0);
					break;

				case EDirectionEnum::DE_RIGHT:
					VectorDirection = FVector(0, 1, 0);
					break;
				
				case EDirectionEnum::DE_LEFT:
					VectorDirection = FVector(0, -1, 0);
					break;

				case EDirectionEnum::DE_UP:
					VectorDirection = FVector(0, 0, 1);
					break;
				
				case EDirectionEnum::DE_DOWN:
					VectorDirection = FVector(0, 0, -1);
					break;

				default:
					VectorDirection = FVector(0, 0, 0);
			}


			float Dist = FMath::RandRange(DistMin, DistMax);
			FVector NewPoint = Point + Dist * VectorDirection;

			TempPoints.Add(NewPoint);
			TempDirections.Add(Direction);

			NewEndPoints.Add(NewPoint);
			

			//int32 index = EndPoints.Add(NewPoint);
			//EndPointDirections.Insert(Direction, index);
		}

		Passages.Add(FPassage(Point, NewEndPoints));

		
		//int32 index = EndPoints.Remove(Point);
		//EndPointDirections.RemoveAt(index);
		
	}

	EndPoints = TempPoints;
	EndPointDirections = TempDirections;
}

EDirectionEnum AMazeGenerator::ChooseRandomDirection(TArray<EDirectionEnum> Exclude)
{
	int32 Rand = FMath::RandRange(0, 5);
	bool found = false;

	EDirectionEnum Direction = EDirectionEnum::DE_ERROR;

	while (!found)
	{
		Direction = static_cast<EDirectionEnum>(FMath::RandRange(0, 5));
		if (!Exclude.Contains(Direction))
		{
			found = true;
		}
	}

	return Direction;
}

void AMazeGenerator::GenerateMeshSections()
{
	for (FPassage Passage : Passages)
	{
		for (FVector EndPoint : Passage.End)
		{
			//Generate the 8 verticies
			FVector Direction = (Passage.Start - EndPoint).GetUnsafeNormal();
			FVector Unused = GetUnusedAxes(Direction);
			FVector Used = GetUsedAxes(Direction);

			TArray<FVector> Verticies;
			//Verticies.Add();
		}
	}
}

FVector AMazeGenerator::GetUsedAxes(FVector Direction)
{
	FVector Used = FVector::ZeroVector;
	if (Direction.X != 0.0f)
	{
		Used.X = Direction.X / FMath::Abs(Direction.X);
	}
	if (Direction.Y != 0.0f)
	{
		Used.Y = Direction.Y / FMath::Abs(Direction.Y);
	}
	if (Direction.Z != 0.0f)
	{
		Used.Z = Direction.Z / FMath::Abs(Direction.Z);
	}

	return Used;
}

FVector AMazeGenerator::GetUnusedAxes(FVector Direction)
{
	FVector Unused = FVector::ZeroVector;
	if (Direction.X == 0.0f)
	{
		Unused.X = 1.0f;
	}
	if (Direction.Y == 0.0f)
	{
		Unused.Y = 1.0f;
	}
	if (Direction.Z == 0.0f)
	{
		Unused.Z = 1.0f;
	}

	return Unused;
}

//TArray<EDirectionEnum> Exclude;
//Exclude.AddUnique(EDirectionEnum::DE_DOWN);
//EDirectionEnum Direction = ChooseRandomDirection(Exclude);

//FString TextDirection = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDirectionEnum"), true)->GetDisplayNameText(static_cast<int32>(Direction)).ToString();
//UE_LOG(LogTemp, Warning, TEXT("%s"), *TextDirection);