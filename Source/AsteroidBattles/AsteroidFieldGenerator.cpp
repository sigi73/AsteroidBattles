// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidBattles.h"
#include "Kismet/KismetMathLibrary.h"
#include "AsteroidFieldGenerator.h"


// Sets default values
AAsteroidFieldGenerator::AAsteroidFieldGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	MovementForce = 100.0f;

	MinScale = 1.0f;
	MaxScale = 1.0f;
}

// Called when the game starts or when spawned
void AAsteroidFieldGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAsteroidFieldGenerator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	RemoveAsteroids();
	AddAsteroids();

}

void AAsteroidFieldGenerator::RemoveAsteroids()
{
	FBox Box = BoxComponent->CalcBounds(GetTransform()).GetBox();
	for (int i = 0; i < Asteroids.Num(); i++)
	{
		if (!Box.IsInsideOrOn(Asteroids[i]->GetActorLocation()))
		{
			AGenericAsteroid* Asteroid = Asteroids[i];
			Asteroids.RemoveAt(Asteroids.Find(Asteroid));
			Asteroid->Destroy();
		}
			
	}
}

void AAsteroidFieldGenerator::AddAsteroids()
{
	FBox Box = BoxComponent->CalcBounds(GetTransform()).GetBox();
	FVector Center, Extents;
	Box.GetCenterAndExtents(Center, Extents);

	while (Asteroids.Num() < AsteroidNum)
	{
		FVector Location = Center;
		FVector ForceDirection;

		switch ((FMath::RandRange(0, 5)))
		{
			case 0: //+x
				Location.X += Extents.X;
				Location.Y += FMath::RandRange(-Extents.Y, Extents.Y);
				Location.Z += FMath::RandRange(-Extents.Z, Extents.Z);
				ForceDirection = FVector(-1, 0, 0);
				break;
			case 1: //-x
				Location.X -= Extents.X;
				Location.Y += FMath::RandRange(-Extents.Y, Extents.Y);
				Location.Z += FMath::RandRange(-Extents.Z, Extents.Z);
				ForceDirection = FVector(1, 0, 0);
				break;
			case 2: //+y
				Location.X += FMath::RandRange(-Extents.X, Extents.X);
				Location.Y += Extents.Y;
				Location.Z += FMath::RandRange(-Extents.Z, Extents.Z);
				ForceDirection = FVector(0, -1, 0);
				break;
			case 3: //-y
				Location.X += FMath::RandRange(-Extents.X, Extents.X);
				Location.Y -= Extents.Y;
				Location.Z += FMath::RandRange(-Extents.Z, Extents.Z);
				ForceDirection = FVector(0, 1, 0);
				break;
			case 4: //+z
				Location.X += FMath::RandRange(-Extents.X, Extents.X);
				Location.Y += FMath::RandRange(-Extents.Y, Extents.Y);
				Location.Z += Extents.Z;
				ForceDirection = FVector(0, 0, -1);
				break;
			case 5: //-z
			default:
				Location.X += FMath::RandRange(-Extents.X / 2.0f, Extents.X / 2.0f);
				Location.Y += FMath::RandRange(-Extents.Y / 2.0f, Extents.Y / 2.0f);
				Location.Z -= Extents.Z;
				ForceDirection = FVector(0, 0, 1);
				break;
		}
		
		
		AGenericAsteroid* NewAsteroid = Cast<AGenericAsteroid>(GetWorld()->SpawnActor(AsteroidClasses[FMath::RandRange(0, AsteroidClasses.Num() - 1)]));
		if (NewAsteroid)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Direction: %s"), *ForceDirection.ToString());
			NewAsteroid->SetActorLocation(Location);
			float NewScaleMultiplier = FMath::FRandRange(MinScale, MaxScale);
			FVector NewScale = FVector(NewScaleMultiplier);
			NewAsteroid->SetScale(NewScale);
			NewAsteroid->AsteroidMesh->AddRelativeRotation(UKismetMathLibrary::RandomRotator(true));
			//NewAsteroid->Move(ForceDirection, 10000000.0f);
			NewAsteroid->Move(ForceDirection, MovementForce);
			Asteroids.Add(NewAsteroid);
		}
	}
}
