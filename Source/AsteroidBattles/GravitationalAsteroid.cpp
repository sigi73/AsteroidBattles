// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidBattles.h"
#include "GravitationalAsteroid.h"


// Sets default values
AGravitationalAsteroid::AGravitationalAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGravitationalAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravitationalAsteroid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	//For each other object, add a force towards the other object that is proportional to the inverse square of the distance.
	//Requirese measurment of mass. Use built in mass? Or have seperate masses?
}

float AGravitationalAsteroid::GetMass()
{
	return AsteroidMesh->GetMass();
}