// Fill out your copyright notice in the Description page of Project Settings.

#include "AsteroidBattles.h"
#include "BaseSpaceship.h"
#include "BaseAIShipController.h"
#include "SquadronManager.h"

USquadronManager::USquadronManager()
{
	ForwardOffset = -100.0f;
	HorizontalOffset = 0.0f;

	bShouldTick = false;

	//UE_LOG(LogTemp, Warning, TEXT("CONSTRUCTED"));
}

bool USquadronManager::IsTickable() const
{
	//return false;
	//return GWorld->HasBegunPlay() && !GIsEditor;
	return bShouldTick;
}

TStatId USquadronManager::GetStatId() const
{
	return TStatId();
}

void USquadronManager::SetLeader(ABaseSpaceship* NewLeader)
{
	Leader = NewLeader;
}

void USquadronManager::Tick(float DeltaSeconds)
{
	if (Leader != NULL)
	{
		FVector LeaderLocation = Leader->GetActorLocation();
		FRotator LeaderRotation = Leader->GetActorRotation();
		FVector LeaderForward = Leader->GetActorForwardVector();
		FVector LeaderRight = Leader->GetActorRightVector();
		FVector CurrentOffset = FVector::ZeroVector;
		float TotalForwardOffset = 0.0f;
		float TotalHorizontalOffset = 0.0f;

		int i = 0;

		for (ABaseSpaceship* CurrentShip : SquadronShips)
		{
			i++;
			//UE_LOG(LogTemp, Warning, TEXT("Going through array"));
			if (CurrentShip->GetUniqueID() == Leader->GetUniqueID())
				continue;

			TotalForwardOffset += ForwardOffset;
			TotalHorizontalOffset += HorizontalOffset;

			CurrentOffset += FVector(-ForwardOffset, HorizontalOffset, 0.0f);


			ABaseAIShipController* CurrentController = Cast<ABaseAIShipController>(CurrentShip->GetController());
			if (CurrentController != NULL)
			{
				CurrentController->TargetLocation = LeaderLocation + (LeaderForward / LeaderForward.Size()) * TotalForwardOffset + (LeaderRight / LeaderRight.Size()) * TotalHorizontalOffset;
				CurrentController->TargetRotation = LeaderRotation;
				//UE_LOG(LogTemp, Warning, TEXT("Setting Locations"));
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Controller is null"));
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("%d"), i);
	}
}


