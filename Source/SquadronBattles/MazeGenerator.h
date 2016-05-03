// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UENUM(BlueprintType)
enum class EDirectionEnum : uint8
{
	DE_UP,
	DE_DOWN,
	DE_LEFT,
	DE_RIGHT,
	DE_FORWARD,
	DE_BACKWARD,
	DE_ERROR
};

USTRUCT(BlueprintType)
struct FPassage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Start;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> End;

	FPassage()
	{
		Start = FVector::ZeroVector;
	}

	FPassage(FVector StartPoint, TArray<FVector> EndPoint)
	{
		Start = StartPoint;
		End = EndPoint;
	}
};


/**
 * 
 */
UCLASS()
class SQUADRONBATTLES_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Generation")
	void GenerateMaze(int32 MinEnd, int32 MaxEnd, float DistMin, float DistMax, FVector Start);
	
protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Generation")
	TArray<FVector> EndPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Generation")
	TArray<EDirectionEnum> EndPointDirections;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Generation")
	TArray<FPassage> Passages;

private:

	int32 MinEndPoints;
	int32 MaxEndPoints;

	FVector Start;

	float DistMin;
	float DistMax;

	void AddEndPoints();
	EDirectionEnum ChooseRandomDirection(TArray<EDirectionEnum> Exclude);
	void GenerateMeshSections();
};
