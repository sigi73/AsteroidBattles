// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SquadronManager.generated.h"

/**
 * 
 */
UCLASS()
class SQUADRONBATTLES_API USquadronManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	USquadronManager();

	virtual void Tick(float DeltaSeconds) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Formations")
	TArray<class ABaseSpaceship*> SquadronShips;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Formations")
	float ForwardOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Formations")
	float HorizontalOffset;

	UFUNCTION(BlueprintCallable, Category = "Formation")
	void SetLeader(ABaseSpaceship* NewLeader);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Test")
	bool bShouldTick;

private:
	ABaseSpaceship* Leader;
};
