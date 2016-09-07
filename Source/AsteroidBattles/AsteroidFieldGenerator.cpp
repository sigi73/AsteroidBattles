// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidBattles.h"
#include "AsteroidFieldGenerator.h"


// Sets default values
AAsteroidFieldGenerator::AAsteroidFieldGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;


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
	for (AActor* Asteroid : Asteroids)
	{
		if (!Box.IsInsideOrOn(Asteroid->GetActorLocation()))
		{
			Asteroids.Remove(Asteroid);
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
				Location.Z -= Extents.Z / 2.0f;
				ForceDirection = FVector(0, 0, 1);
				break;
		}
		
		
		AActor* NewAsteroid = GetWorld()->SpawnActor(AsteroidClasses[FMath::RandRange(0, AsteroidClasses.Num() - 1)]);
		NewAsteroid->SetActorLocation(Location);
		Asteroids.Add(NewAsteroid);
	}
}
