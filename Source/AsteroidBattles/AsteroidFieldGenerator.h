// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AsteroidFieldGenerator.generated.h"

UCLASS()
class ASTEROIDBATTLES_API AAsteroidFieldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidFieldGenerator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Asteroids;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UClass*> AsteroidClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AsteroidNum;

private:
	void RemoveAsteroids();
	void AddAsteroids();
	
};
