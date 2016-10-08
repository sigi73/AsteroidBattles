// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidBattles.h"
#include "GenericAsteroid.h"


// Sets default values
AGenericAsteroid::AGenericAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AsteroidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = AsteroidMesh;
}

// Called when the game starts or when spawned
void AGenericAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
	AsteroidMesh->SetSimulatePhysics(true);
	AsteroidMesh->WakeRigidBody();
	AsteroidMesh->SetEnableGravity(false);
	AsteroidMesh->SetMassOverrideInKg("", 0.01f, true);
}

// Called every frame
void AGenericAsteroid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AGenericAsteroid::Move(FVector Direction, float Magnitude)
{
	//AsteroidMesh->SetSimulatePhysics(true);
	//UE_LOG(LogTemp, Warning, TEXT("Direction * Magnitude: %s"), *(Direction * Magnitude).ToString());
	AsteroidMesh->AddForce(Direction * Magnitude);
	//AsteroidMesh->AddImpulse(Direction * Magnitude);
}

void AGenericAsteroid::SetScale(FVector NewScale)
{
	AsteroidMesh->SetWorldScale3D(NewScale);
}